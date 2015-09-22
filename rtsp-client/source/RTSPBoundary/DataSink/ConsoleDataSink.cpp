#include "stdafx.h"
#include "ConsoleDataSink.h"

const std::size_t gReceiveBufferSize = 100000;

ssc::ConsoleDataSink::ConsoleDataSink(UsageEnvironment &env, MediaSubsession &subsession)
    : MediaSink(env)
    , mReceiveBuffer(gReceiveBufferSize)
    , mSubSession(subsession)
{
}

bool ssc::ConsoleDataSink::continuePlaying()
{
    if (!fSource)
        return false; // sanity check (should not happen)

                                       // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
    fSource->getNextFrame(&mReceiveBuffer[0], mReceiveBuffer.size(),
        afterGettingFrame, this,
        onSourceClosure, this);
    return true;
}

void ssc::ConsoleDataSink::afterGettingFrame(void *clientData, unsigned frameSize,
    unsigned numTruncatedBytes, timeval presentationTime, unsigned durationInMicroseconds)
{
    ConsoleDataSink* sink = (ConsoleDataSink*)clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

void ssc::ConsoleDataSink::afterGettingFrame(unsigned int frameSize, unsigned int numTruncatedBytes,
    timeval presentationTime, unsigned int durationInMicroseconds)
{
    envir() << mSubSession.mediumName() << "/" << mSubSession.codecName() << ":\tReceived " << frameSize << " bytes";
    if (numTruncatedBytes > 0)
        envir() << " (with " << numTruncatedBytes << " bytes truncated)";

    char uSecsStr[6 + 1]; // used to output the 'microseconds' part of the presentation time
    std::snprintf(uSecsStr, sizeof(uSecsStr), "%06u", (unsigned)presentationTime.tv_usec);

    envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr << "\n";
    if (mSubSession.rtpSource() && !mSubSession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
        envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
    }

    // Then continue, to request the next frame of data
    continuePlaying();
}
