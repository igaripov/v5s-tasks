#pragma once

#include "RTSPBoundary/Callbacks/DescribeCallback.h"
#include "RTSPBoundary/RTSPController.h"

namespace ssc {

class ADataSinkFactory;
class RTSPConnection;
class AExecutor; 
class Streams;
class Stream;

class IExecutorCallback
{
public:
    virtual void executorDone(RTSPConnection &connection) = 0;
    virtual void executorChanged(RTSPConnection &connection, AExecutor *newExecutor) = 0;
    virtual MediaSink * createDataSink(Stream &stream) = 0;
    virtual RTSPController & getRTSPController() = 0;

    virtual ~IExecutorCallback() {}
};

class Streamer : private IExecutorCallback
{
public:
    Streamer(ADataSinkFactory &sinkFactory);
    ~Streamer();

    void addURL(const char *url);
    void startStreaming();
private:
    // Inherited via IExecutorCallback
    virtual void executorDone(RTSPConnection &connection) override;
    virtual void executorChanged(RTSPConnection &connection, AExecutor *newExecutor) override;
    virtual MediaSink * createDataSink(Stream & stream) override;
    virtual RTSPController & getRTSPController() override;

    RTSPConnection & createConnection(const char *url);
    void discribe(RTSPConnection &connection);
    void setupStreams(Streams &streams);
    void runStreaming(Streams &streams);

    ADataSinkFactory &mDataSinkFactory;
    typedef std::unordered_map<std::shared_ptr<RTSPConnection>, std::shared_ptr<AExecutor>> Executors;
    Executors mExecutors;
    RTSPController mRTSPController;
};

} // ~namespace ssc