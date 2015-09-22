#include "stdafx.h"
#include "Player.h"
#include "Streamer/Streamer.h"
#include "RTSPBoundary/RTSPController.h"
#include "RTSPBoundary/Commands/RTSPCommand.h"
#include "RTSPBoundary/Commands/RTSPCommandFactory.h"

ssc::Player::Player(IExecutorCallback & owner, RTSPConnection & connection)
    : AExecutor(owner, connection)
{
}

void ssc::Player::execute()
{
    RTSPController &controller = mOwner.getRTSPController();
    // resource info
    RTSPCommand command = RTSPCommandFactory::createPlay(mConnection, *this);
    controller.postCommand(command);
}

void ssc::Player::failed(RTSPConnection * connection, const char * error)
{
    mOwner.executorDone(mConnection);
}

void ssc::Player::playCompleted()
{
    mOwner.executorDone(mConnection);
}
