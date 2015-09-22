#include "stdafx.h"
#include "Stream.h"

ssc::Stream::Stream(MediaSubsession * subSession)
    : mSubSession(subSession)
{
}

MediaSubsession * ssc::Stream::getSubSession()
{
    return mSubSession;
}
