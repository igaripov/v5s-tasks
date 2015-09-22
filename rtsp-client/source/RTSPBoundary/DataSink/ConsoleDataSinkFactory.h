#pragma once

#include "ADataSinkFactory.h"

namespace ssc {

class ConsoleDataSinkFactory : public ADataSinkFactory
{
public:

    // Inherited via ADataSinkFactory
    virtual MediaSink * createDataSink(UsageEnvironment &env, Stream & stream) const override;
};

} // ~namespace ssc