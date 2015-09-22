#pragma once

namespace ssc {

class ConsoleDataSink : public MediaSink
{
public:
    // Inherited via MediaSink
    virtual bool continuePlaying() override;

    static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
        timeval presentationTime, unsigned durationInMicroseconds);
    virtual void afterGettingFrame(unsigned int frameSize, unsigned int numTruncatedBytes,
        timeval presentationTime, unsigned int durationInMicroseconds);
private:
    friend class ConsoleDataSinkFactory;
    ConsoleDataSink(UsageEnvironment& env, MediaSubsession& subsession);

    std::vector<uint8_t> mReceiveBuffer;
    MediaSubsession &mSubSession;
};

} // ~namespace ssc
