#pragma once

namespace ssc {

class IExecutorCallback;
class RTSPConnection;

class AExecutor
{
public:
    AExecutor(IExecutorCallback &owner, RTSPConnection &connection);

    virtual void execute() = 0;

    virtual ~AExecutor() = 0 {}
protected:
    IExecutorCallback &mOwner;
    RTSPConnection &mConnection;
};

} // ~namespace ssc
