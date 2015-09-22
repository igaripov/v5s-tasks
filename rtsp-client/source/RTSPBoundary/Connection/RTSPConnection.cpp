#include "stdafx.h"
#include "RTSPConnection.h"
#include "RTSPClientInner.h"
#include "RTSPBoundary/Streams/Streams.h"

ssc::RTSPConnection::RTSPConnection(UsageEnvironment &env, const char *url)
{
    mClient.reset(new RTSPClientInner(*this, env, url, 1 /* verbose */, "rtsp-client"),
        [](Medium *d) { d->close(d); });
}

ssc::RTSPConnection::~RTSPConnection()
{
}

ssc::Streams & ssc::RTSPConnection::getStreams()
{
    return *mStreams;
}
