#include "stdafx.h"
#include "PlayCallback.h"
#include "RTSPBoundary/Connection/RTSPConnection.h"

void ssc::PlayCallback::handle(IRTSPConnectionCallbacks & callback)
{
    callback.onPlay(*this);
}
