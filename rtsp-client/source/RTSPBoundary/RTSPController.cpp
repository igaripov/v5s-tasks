#include "stdafx.h"
#include "RTSPController.h"
#include "Connection/RTSPConnection.h"
#include "Commands/RTSPCommand.h"

ssc::RTSPController::RTSPController()
    : mExecutionStopped(0)
{
    mScheduler.reset(BasicTaskScheduler::createNew());
    mEnv.reset(BasicUsageEnvironment::createNew(*mScheduler),
        [](UsageEnvironment *d) { d->reclaim(); });
}

ssc::RTSPController::~RTSPController()
{
}

ssc::RTSPConnection * ssc::RTSPController::createConnect(const char *url)
{
    return new RTSPConnection(*mEnv, url);
}

void ssc::RTSPController::postCommand(RTSPCommand &command)
{
    command.execute();
}

void ssc::RTSPController::executionLoop()
{
    mScheduler->doEventLoop(&mExecutionStopped);
}

void ssc::RTSPController::stop()
{
    mExecutionStopped = 1;
}

UsageEnvironment & ssc::RTSPController::getEnv()
{
    return *mEnv;
}
