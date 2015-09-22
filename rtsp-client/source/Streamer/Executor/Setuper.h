#pragma once

#include "AExecutor.h"
#include "RTSPBoundary/Callbacks/SetupCallback.h"
#include "RTSPBoundary/Commands/RTSPCommand.h"

namespace ssc {

class RTSPController;

class Setuper : public AExecutor, private SetupCallback
{
public:
    Setuper(IExecutorCallback &owner, RTSPConnection &connection);

    // Inherited via AExecutor
    virtual void execute() override;

private:
    // Inherited via SetupCallback
    virtual void failed(RTSPConnection *connection, const char *error) override;
    virtual void setupStream(Stream &stream) override;
    virtual void setupCompleted(Streams *streams) override;

    void initCommands();
    bool postCommand();
    void sinkInitializedStream(Stream &stream);
    void playStreams();

    typedef std::deque<RTSPCommand> Commands;
    Commands mCommands;
};

} // ~namespace ssc
