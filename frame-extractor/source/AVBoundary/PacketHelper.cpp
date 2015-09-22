#include "stdafx.h"
#include "PacketHelper.h"

PacketHelper::PacketHelper()
    : mPkt()
{
    ::av_init_packet(&mPkt);
    mPkt.data = nullptr;
    mPkt.size = 0;
}

PacketHelper::~PacketHelper()
{
    ::av_free_packet(&mPkt);
}

AVPacket * PacketHelper::getPkt()
{
    return &mPkt;
}

void PacketHelper::reset()
{
    ::av_free_packet(&mPkt);
    mPkt.data = nullptr;
    mPkt.size = 0;
}
