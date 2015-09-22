#include "stdafx.h"
#include "DecoderHelper.h"

DecoderHelper::DecoderHelper(IAVErrorHandler &handler)
    : mErrorHandler(handler)
    , mCodec(nullptr)
    , mCodecContext(nullptr)
{
}

DecoderHelper::~DecoderHelper()
{
    if (mCodecContext)
    {
        ::avcodec_close(mCodecContext);
        ::av_free(mCodecContext);
    }
}

bool DecoderHelper::attach(AVCodecID codecID)
{
    mCodec = ::avcodec_find_decoder(codecID);
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

    int result = ::avcodec_open2(mCodecContext, mCodec, nullptr);
    if (0 > result)
    {
        mErrorHandler.onErrorOccurred("Could not open codec for decoding");
        return false;
    }

    return true;
}

AVCodecContext * DecoderHelper::getContext()
{
    return mCodecContext;
}

bool DecoderHelper::decode(uint8_t *data, int size, FrameHelper &frame)
{
    AVPacket packet;
    int isPicture = 0;

    ::av_init_packet(&packet);

    packet.data = data;
    packet.size = size;
    
    int result = ::avcodec_decode_video2(mCodecContext, frame.getPicture(), &isPicture, &packet);
    ::av_free_packet(&packet);

    if (result < 0) {
        mErrorHandler.onErrorOccurred("Error while decoding a frame.\n");
        return false;
    }

    if (isPicture == 0) {
        return false;
    }
    return true;
}
