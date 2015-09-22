#pragma once

namespace ssc {

class Stream;

class ADataSinkFactory
{
public:
    virtual MediaSink * createDataSink(UsageEnvironment &env, Stream &stream) const = 0;
    virtual ~ADataSinkFactory() = 0 {}
};

} // ~namespace ssc