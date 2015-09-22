#pragma once

#include "ARTSPCommandCallback.h"

namespace ssc {

class Stream;
class Streams;
class RTSPConnection;

class SetupCallback : public ARTSPCommandCallback
{
public:
    virtual void setupStream(Stream &stream) = 0;
    virtual void setupCompleted(Streams *streams) = 0;

    virtual ~SetupCallback() {}

    // Inherited via ARTSPCommandCallback
    virtual void handle(IRTSPConnectionCallbacks &callback) override;
};
} // ~namespace ssc
