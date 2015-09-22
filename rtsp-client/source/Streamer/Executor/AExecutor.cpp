#include "stdafx.h"
#include "AExecutor.h"

ssc::AExecutor::AExecutor(IExecutorCallback &owner, RTSPConnection &connection)
    : mOwner(owner)
    , mConnection(connection)
{
}
