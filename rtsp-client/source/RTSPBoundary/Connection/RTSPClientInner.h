#pragma once

#include "RTSPConnection.h"

namespace ssc {

class ARTSPCommandCallback;
class Stream;

class RTSPConnection::RTSPClientInner : public RTSPClient, public IRTSPConnectionCallbacks
{
public:
    static void responseHandler(RTSPClient *rtspClient, int resultCode, char *resultString);
    static void subsessionClose(void *clientData);

    ARTSPCommandCallback *mCurrentCallback;
    Stream *mStream;

    RTSPClientInner(RTSPConnection &parent, UsageEnvironment &env, const char *rtspURL, int verbosityLevel = 0,
        const char *applicationName = nullptr, portNumBits tunnelOverHTTPPortNum = 0,
        int socketNumToServer = -1);
private:
    struct Response;

    std::unique_ptr<Streams> createStreams(const char *spdDescription);
    void tearDownStreams();

    // Inherited via IRTSPConnectionCallbacks
    virtual void onDescribe(DescribeCallback &callback) override;
    virtual void onSetup(SetupCallback & callback) override;
    virtual void onPlay(PlayCallback & callback) override;

    RTSPConnection &mParentConnection;
    Response *mCurrentResponse;
};

} // ~namespace ssc