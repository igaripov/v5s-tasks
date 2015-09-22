#pragma once

class FrameHelper
{
public:
    FrameHelper();
    ~FrameHelper();

    AVFrame * getPicture();
private:
    AVFrame *mFrame;
};

