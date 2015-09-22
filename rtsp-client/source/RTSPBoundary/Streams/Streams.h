#pragma once

#include "Stream.h"
#include "RTSPBoundary/Connection/RTSPConnection.h"

namespace ssc {

class Streams
{
public:
    Streams(MediaSession *session);
    ~Streams();

    std::size_t size();
    Stream & get(std::size_t index);
    bool hasStreams();
    double & duration();

    MediaSession * getMediaSession();

    std::unique_ptr<MediaSubsessionIterator> getStreams();
private:
    friend class RTSPCommandFactory;

    MediaSession *mSession;
    std::unique_ptr<MediaSubsessionIterator> mSubsessionIterator;
    std::deque<Stream> mStreams;
    double mDuration;
};

} // ~namespace ssc