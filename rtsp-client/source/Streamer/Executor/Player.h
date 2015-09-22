#pragma once

#include "AExecutor.h"
#include "RTSPBoundary/Callbacks/PlayCallback.h"

namespace ssc {
class RTSPConnection;
class RTSPController;

class Player : public AExecutor, private PlayCallback
{
public:
    Player(IExecutorCallback &owner, RTSPConnection &connection);

    // Inherited via AExecutor
    virtual void execute() override;
private:
    // Inherited via PlayCallback
    virtual void failed(RTSPConnection *connection, const char *error) override;
    virtual void playCompleted() override;
};

} // ~namespace ssc
