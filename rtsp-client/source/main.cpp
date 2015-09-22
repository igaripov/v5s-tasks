// rtsp-client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RTSPBoundary/DataSink/ConsoleDataSinkFactory.h"
#include "Streamer/Streamer.h"

using namespace ssc;

static void usage(const char *name);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv[0]);
        return -1;
    }

    const char *url = argv[1];

    // streaming resource
    ConsoleDataSinkFactory sinkFactory;
    Streamer streamer(sinkFactory);
    streamer.addURL(url);
    streamer.startStreaming();

    return 0;
}

static void usage(const char *name)
{
    std::printf("Usage: %s <rtsp-url>\t(where <rtsp-url> is a \"rtsp://\" URL)\n", name);
}

