#include "stdafx.h"
#include "Describer.h"
#include "Setuper.h"
#include "Streamer/Streamer.h"
#include "RTSPBoundary/RTSPController.h"
#include "RTSPBoundary/Commands/RTSPCommand.h"
#include "RTSPBoundary/Commands/RTSPCommandFactory.h"

ssc::Describer::Describer(IExecutorCallback &owner, RTSPConnection &connection)
	: AExecutor(owner, connection)
{

}

void ssc::Describer::execute()
{
    RTSPController &controller = mOwner.getRTSPController();
    // resource info
    RTSPCommand command = RTSPCommandFactory::createDescribe(mConnection, *this);
    controller.postCommand(command);
}

void ssc::Describer::failed(RTSPConnection *connection, const char *error)
{
    mOwner.executorDone(mConnection);
}

void ssc::Describer::describeStreams(Streams &streams)
{
    auto setuper = std::make_unique<Setuper>(mOwner, mConnection);
    mOwner.executorChanged(mConnection, setuper.release());
}
