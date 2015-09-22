#pragma once
#include "AExecutor.h"
#include "RTSPBoundary/Callbacks/DescribeCallback.h"

namespace ssc {

class RTSPConnection;
class RTSPController;

class Describer : public AExecutor, private DescribeCallback
{
public:
    Describer(IExecutorCallback &owner, RTSPConnection &connection);

    // Inherited via AExecutor
    virtual void execute() override;
private:
    // Inherited via DescribeCallback
    virtual void failed(RTSPConnection *connection, const char *error) override;
    virtual void describeStreams(Streams &streams) override;
};

} // ~namespace ssc
