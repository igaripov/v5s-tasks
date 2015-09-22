#pragma once

class PacketHelper
{
public:
    PacketHelper();
    ~PacketHelper();
    AVPacket * getPkt();
    void reset();
private:
    AVPacket mPkt;
};

