#include "stdafx.h"
#include "DescribeCallback.h"
#include "RTSPBoundary/Connection/RTSPConnection.h"

void ssc::DescribeCallback::handle(IRTSPConnectionCallbacks &callback)
{
	callback.onDescribe(*this);
}
