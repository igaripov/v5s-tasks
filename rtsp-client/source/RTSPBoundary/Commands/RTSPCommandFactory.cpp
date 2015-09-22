#include "stdafx.h"
#include "RTSPCommandFactory.h"
#include "RTSPCommand.h"
#include "RTSPBoundary/Streams/Stream.h"
#include "RTSPBoundary/Streams/Streams.h"
#include "RTSPBoundary/Connection/RTSPClientInner.h"
#include "RTSPBoundary/Callbacks/DescribeCallback.h"
#include "RTSPBoundary/Callbacks/SetupCallback.h"
#include "RTSPBoundary/Callbacks/PlayCallback.h"

ssc::RTSPCommand ssc::RTSPCommandFactory::createDescribe(RTSPConnection &connection, DescribeCallback &callback)
{
    RTSPConnection::RTSPClientInner *client = connection.mClient.get();
    client->mCurrentCallback = &callback;
    std::function<void()> describeHandler = [client]() {
        client->sendDescribeCommand(&RTSPConnection::RTSPClientInner::responseHandler);
    };

    return RTSPCommand(describeHandler);
}

ssc::RTSPCommand ssc::RTSPCommandFactory::createSetup(RTSPConnection &connection, Stream &stream, SetupCallback &callback)
{
    RTSPConnection::RTSPClientInner *client = connection.mClient.get();
    client->mCurrentCallback = &callback;
    client->mStream = &stream;
    std::function<void()> setupHandler = [client, &connection]() {
        auto subSession = client->mStream->getSubSession();
        if (!subSession->initiate()) {
            client->envir() << "Failed to initiate the \"" << subSession->mediumName()
                << "/" << subSession->codecName()
                << "\" subsession: " << client->envir().getResultMsg() << "\n";
            client->mCurrentCallback->failed(&connection, "");
            return;
        }
        client->sendSetupCommand(*subSession, &RTSPConnection::RTSPClientInner::responseHandler);
    };

    return RTSPCommand(setupHandler);
}

ssc::RTSPCommand ssc::RTSPCommandFactory::createPlay(RTSPConnection &connection, PlayCallback &callback)
{
    RTSPConnection::RTSPClientInner *client = connection.mClient.get();
    client->mCurrentCallback = &callback;
    client->mStream = nullptr;

    auto playStreams = [](RTSPConnection::RTSPClientInner *client, MediaSubsession *subsession) {
        client->envir() << "Created a data sink for the \"" << subsession->mediumName()
            << "/" << subsession->codecName() << "\" subsession\n";
        subsession->miscPtr = client; // a hack to let subsession handler functions get the "RTSPClient" from the subsession 
        subsession->sink->startPlaying(*(subsession->readSource()),
            RTSPConnection::RTSPClientInner::subsessionClose, subsession);
        // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
        if (subsession->rtcpInstance() != NULL) {
            subsession->rtcpInstance()->setByeHandler(&RTSPConnection::RTSPClientInner::subsessionClose, subsession);
        }
    };

    std::function<void()> playHandler = [client, &connection, playStreams]() {
        Streams &streams = connection.getStreams();
        auto iterator = streams.getStreams();
        MediaSubsession *substream;
        while (nullptr != (substream = iterator->next()))
        {
            playStreams(client, substream);
        }

        MediaSession *session = streams.mSession;
        // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
        if (session->absStartTime() != NULL) {
            // Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
            client->sendPlayCommand(*session, &RTSPConnection::RTSPClientInner::responseHandler,
                session->absStartTime(), session->absEndTime());
        } else {
            streams.duration() = session->playEndTime() - session->playStartTime();
            client->sendPlayCommand(*session, &RTSPConnection::RTSPClientInner::responseHandler);
        }
    };

    return RTSPCommand(playHandler);
}
