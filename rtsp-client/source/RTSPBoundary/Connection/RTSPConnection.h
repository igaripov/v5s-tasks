#pragma once

namespace ssc {

class DescribeCallback;
class SetupCallback;
class PlayCallback;
class Streams;

class IRTSPConnectionCallbacks
{
public:
    virtual void onDescribe(DescribeCallback &callback) = 0;
    virtual void onSetup(SetupCallback &callback) = 0;
    virtual void onPlay(PlayCallback &callback) = 0;
protected:
    virtual ~IRTSPConnectionCallbacks() { }
};

class RTSPConnection
{
public:
    ~RTSPConnection();

    Streams & getStreams();
private:
    class RTSPClientInner;

    friend class RTSPController;
    friend class RTSPCommandFactory;
    friend class RTSPClientInner;

    RTSPConnection(UsageEnvironment &env, const char *url);

    std::shared_ptr<RTSPClientInner> mClient;
    std::unique_ptr<Streams> mStreams;
};

} // ~namespace ssc