#include "stdafx.h"
#include "RTSPClientInner.h"
#include "RTSPBoundary/Streams/Streams.h"
#include "RTSPBoundary/Callbacks/SetupCallback.h"
#include "RTSPBoundary/Callbacks/DescribeCallback.h"
#include "RTSPBoundary/Callbacks/PlayCallback.h"

struct ssc::RTSPConnection::RTSPClientInner::Response
{
    int resultCode;
    char *resultString;
    bool isSessionClosing;

    ~Response()
    {
        delete[] resultString;
    }
};

void ssc::RTSPConnection::RTSPClientInner::responseHandler(RTSPClient *rtspClient, int resultCode, char *resultString)
{
    Response response = { resultCode, resultString, false };
    auto &inner = *(RTSPClientInner *)rtspClient;
    inner.mCurrentResponse = &response;
    if (inner.mCurrentCallback)
        inner.mCurrentCallback->handle(inner);
}

void ssc::RTSPConnection::RTSPClientInner::subsessionClose(void * clientData)
{
    Response response = { 0, nullptr, true };
    MediaSubsession *subsession = (MediaSubsession *)clientData;
    auto &inner = *(RTSPClientInner *)subsession->miscPtr;
    inner.mCurrentResponse = &response;

    // Begin by closing this subsession's stream:
    Medium::close(subsession->sink);
    subsession->sink = nullptr;

    // Next, check whether *all* subsessions' streams have now been closed:
    MediaSession& session = subsession->parentSession();
    MediaSubsessionIterator iter(session);
    while ((subsession = iter.next()) != nullptr) {
        if (subsession->sink != nullptr)
            return; // this subsession is still active
    }

    if (inner.mCurrentCallback)
        inner.mCurrentCallback->handle(inner);
}

ssc::RTSPConnection::RTSPClientInner::RTSPClientInner(RTSPConnection &parent, UsageEnvironment &env, const char *rtspURL,
    int verbosityLevel, const char *applicationName, portNumBits tunnelOverHTTPPortNum, int socketNumToServer)
    : RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, socketNumToServer)
    , mCurrentCallback(nullptr)
    , mStream(nullptr)
    , mParentConnection(parent)
    , mCurrentResponse(nullptr)
{

}

std::unique_ptr<ssc::Streams> ssc::RTSPConnection::RTSPClientInner::createStreams(const char *spdDescription)
{
    auto session = MediaSession::createNew(envir(), spdDescription);

    std::unique_ptr<Streams> streams = std::make_unique<Streams>(session);

    if (session == NULL) {
        envir() << "[URL:\"" << url() << "\"]: " << "Failed to create a MediaSession object from the SDP description: " << envir().getResultMsg() << "\n";
        //break;
    } else if (!session->hasSubsessions()) {
        envir() << "[URL:\"" << url() << "\"]: " << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
        //break;
    }

    return streams;
}

void ssc::RTSPConnection::RTSPClientInner::onDescribe(DescribeCallback &callback)
{
    UsageEnvironment& env = envir();
    if (mCurrentResponse->resultCode != 0) {
        env << "[URL:\"" << url() << "\"]: " << "Failed to get a SDP description: " << mCurrentResponse->resultString << "\n";
        callback.failed(&mParentConnection, mCurrentResponse->resultString);
        return;
    }

    mParentConnection.mStreams = createStreams(mCurrentResponse->resultString);
    if (!mParentConnection.mStreams) {
        env << "[URL:\"" << url() << "\"]: " << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
        callback.failed(&mParentConnection, mCurrentResponse->resultString);
        return;
    } else if (!mParentConnection.mStreams->hasStreams()) {
        env << "[URL:\"" << url() << "\"]: " << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
        callback.failed(&mParentConnection, mCurrentResponse->resultString);
        return;
    }
    callback.describeStreams(mParentConnection.getStreams());
}

void ssc::RTSPConnection::RTSPClientInner::onSetup(SetupCallback &callback)
{
    auto subSession = mStream->getSubSession();
    UsageEnvironment& env = envir();
    if (mCurrentResponse->resultCode != 0) {
        env << "[URL:\"" << url() << "\"]: " "Failed to set up the \"" << subSession->mediumName()
            << "/" << subSession->codecName()
            << "\" subsession: " << mCurrentResponse->resultString << "\n";
        callback.failed(&mParentConnection, mCurrentResponse->resultString);
        return;
    }

    env << "[URL:\"" << url() << "\"]: " "Set up the \"" << subSession->mediumName()
        << "/" << subSession->codecName() << "\" subsession (";
    if (subSession->rtcpIsMuxed()) {
        env << "client port " << subSession->clientPortNum();
    } else {
        env << "client ports " << subSession->clientPortNum() << "-" << subSession->clientPortNum() + 1;
    }
    env << ")\n";

    callback.setupStream(*mStream);
}

void ssc::RTSPConnection::RTSPClientInner::onPlay(PlayCallback &callback)
{
    UsageEnvironment& env = envir();

    if (mCurrentResponse->isSessionClosing)
    {
        tearDownStreams();
        callback.playCompleted();
        return;
    }

    if (mCurrentResponse->resultCode != 0) {
        env << "[URL:\"" << url() << "\"]: " "Failed to start playing session: " << mCurrentResponse->resultString << "\n";
        callback.failed(&mParentConnection, mCurrentResponse->resultString);
        return;
    }

    env << "[URL:\"" << url() << "\"]: " "Started playing session";
    double &duration = mParentConnection.getStreams().duration();
    if (duration > 0) {
        env << " (for up to " << duration << " seconds)";
    }
    env << "...\n";
}

void ssc::RTSPConnection::RTSPClientInner::tearDownStreams()
{
    Streams &streams = mParentConnection.getStreams();

    bool someSubsessionsWereActive = false;
    MediaSubsessionIterator iter(*streams.getMediaSession());
    MediaSubsession* subsession;

    while ((subsession = iter.next()) != nullptr) {
        if (subsession->sink != nullptr)
        {
            Medium::close(subsession->sink);
            subsession->sink = nullptr;

            if (subsession->rtcpInstance() != nullptr)
            {
                subsession->rtcpInstance()->setByeHandler(nullptr, nullptr);
            }

            someSubsessionsWereActive = true;
        }
    }

    if (someSubsessionsWereActive) {
        // Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
        // Don't bother handling the response to the "TEARDOWN".
        sendTeardownCommand(*streams.getMediaSession(), nullptr);
    }

    envir() << "[URL:\"" << url() << "\"]: " "Closing the stream.\n";
}

