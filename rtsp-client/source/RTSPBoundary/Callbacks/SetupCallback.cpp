#include "stdafx.h"
#include "SetupCallback.h"
#include "RTSPBoundary/Connection/RTSPConnection.h"

void ssc::SetupCallback::handle(IRTSPConnectionCallbacks &callback)
{
    callback.onSetup(*this);
}
