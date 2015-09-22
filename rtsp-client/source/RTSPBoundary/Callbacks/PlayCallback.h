#pragma once

#include "ARTSPCommandCallback.h"

namespace ssc {

class Streams;
class RTSPConnection;

class PlayCallback : public ARTSPCommandCallback
{
public:
    virtual void playCompleted() = 0;


    virtual ~PlayCallback() {}

    // Inherited via ARTSPCommandCallback
    virtual void handle(IRTSPConnectionCallbacks &callback) override;
};

} // ~namespace ssc
