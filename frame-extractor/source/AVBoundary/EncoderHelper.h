#pragma once

class FrameHelper;
class PacketHelper;

class EncoderHelper
{
public:
    EncoderHelper(IAVErrorHandler &handler);
    ~EncoderHelper();

    bool attach(AVCodecID codecID);
    AVCodecContext * getContext();
    bool encodeFrame(FrameHelper &frame, /*out*/ PacketHelper &packet);
private:
    bool open(FrameHelper &frame);
    IAVErrorHandler &mErrorHandler;
    AVCodec *mCodec;
    AVCodecContext *mCodecContext;
    bool mLazyOpened;
};

