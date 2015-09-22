#pragma once

namespace ssc {

class RTSPCommand;
class RTSPConnection;

class RTSPController
{
public:
    RTSPController();
    ~RTSPController();

    RTSPConnection * createConnect(const char *url);
    void postCommand(RTSPCommand &command);
    void executionLoop();
    void stop();
    UsageEnvironment & getEnv();
private:
    std::shared_ptr<TaskScheduler> mScheduler;
    std::shared_ptr<UsageEnvironment> mEnv;
    volatile char mExecutionStopped;
};

} // ~namespace ssc