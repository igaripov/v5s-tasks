#include "stdafx.h"
#include "FrameHelper.h"

FrameHelper::FrameHelper()
    : mFrame(nullptr)
{
}

FrameHelper::~FrameHelper()
{
    if (mFrame)
    {
        ::av_frame_free(&mFrame);
    }
}

AVFrame * FrameHelper::getPicture()
{
    if (!mFrame)
    {
        mFrame = ::av_frame_alloc();
    }
    return mFrame;
}
