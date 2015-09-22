#pragma once

namespace ssc {

class Stream
{
public:
    Stream(MediaSubsession *subSession);
    MediaSubsession * getSubSession();
private:
    friend class RTSPCommandFactory;
    MediaSubsession *mSubSession;
};

} // ~namespace ssc
