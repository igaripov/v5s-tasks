#pragma once

#include "ARTSPCommandCallback.h"

namespace ssc {

class Streams;
class RTSPConnection;

class DescribeCallback : public ARTSPCommandCallback
{
public:
    virtual void describeStreams(Streams &streams) = 0;

    virtual ~DescribeCallback() {}

    // Inherited via ARTSPCommandCallback
    virtual void handle(IRTSPConnectionCallbacks &callback) override;
};

} // ~namespace ssc
