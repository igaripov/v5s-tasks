#include "stdafx.h"
#include "EncoderHelper.h"

EncoderHelper::EncoderHelper(IAVErrorHandler &handler)
    : mErrorHandler(handler)
    , mCodec(nullptr)
    , mCodecContext(nullptr)
    , mLazyOpened(false)
{
}

EncoderHelper::~EncoderHelper()
{
    if (mCodecContext)
    {
        ::avcodec_close(mCodecContext);
        ::av_free(mCodecContext);
    }
}

bool EncoderHelper::attach(AVCodecID codecID)
{
    mCodec = ::avcodec_find_encoder(codecID);
    if (!mCodec)
    {
        mErrorHandler.onErrorOccurred("Codec not found");
        return false;
    }

    mCodecContext = ::avcodec_alloc_context3(mCodec);
    if (!mCodecContext)
    {
        mErrorHandler.onErrorOccurred("Could not allocate codec context");
        return false;
    }

    return true;
}

AVCodecContext * EncoderHelper::getContext()
{
    return mCodecContext;
}

bool EncoderHelper::encodeFrame(FrameHelper &frame, PacketHelper &packet)
{
    if (!mLazyOpened && !open(frame))
    {
        return false;
    }

    AVFrame *picture = frame.getPicture();
    picture->quality = 1;

    packet.reset();

    int isPacketFilled = 0;
    int status = ::avcodec_encode_video2(mCodecContext, packet.getPkt(), picture, &isPacketFilled);

    if (status < 0) {
        mErrorHandler.onErrorOccurred("Error while encode a frame");
        return false;
    }

    if (!isPacketFilled)
    {
        mErrorHandler.onErrorOccurred("Could not encode a frame");
        return false;
    }

    return true;
}

bool EncoderHelper::open(FrameHelper &frame)
{
    auto picture = frame.getPicture();
    if (!mCodecContext->width)
    {
        mCodecContext->width = picture->width;
    }
    if (!mCodecContext->height)
    {
        mCodecContext->height = picture->height;
    }

    int result = ::avcodec_open2(mCodecContext, mCodec, nullptr);
    if (0 > result)
    {
        mErrorHandler.onErrorOccurred("Could not open codec for encoding");
        return false;
    }
    mLazyOpened = true;
    return true;
}
