#pragma once

class FrameHelper;

class DecoderHelper
{
public:
    DecoderHelper(IAVErrorHandler &handler);
    ~DecoderHelper();

    bool attach(AVCodecID codecID);
    AVCodecContext * getContext();
    bool decode(uint8_t *data, int size, /*out*/ FrameHelper &frame);

private:
    IAVErrorHandler &mErrorHandler;
    AVCodec *mCodec;
    AVCodecContext *mCodecContext;
};

