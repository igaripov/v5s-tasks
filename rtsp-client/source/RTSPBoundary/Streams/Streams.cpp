#include "stdafx.h"
#include "Streams.h"

ssc::Streams::Streams(MediaSession *session)
    : mSession(session)
    , mDuration(0.0)
{
    if (mSession)
    {
        mSubsessionIterator = std::make_unique<MediaSubsessionIterator>(*mSession);
        while (auto subsession = mSubsessionIterator->next())
        {
            mStreams.emplace_back(subsession);
        }
    }
}

ssc::Streams::~Streams()
{
    if (mSession)
        Medium::close(mSession);
}

std::size_t ssc::Streams::size()
{
    return mStreams.size();
}

ssc::Stream & ssc::Streams::get(std::size_t index)
{
    return mStreams[index];
}

bool ssc::Streams::hasStreams()
{
    return !mStreams.empty();
}

double & ssc::Streams::duration()
{
    return mDuration;
}

MediaSession *ssc::Streams::getMediaSession()
{
    return mSession;
}

std::unique_ptr<MediaSubsessionIterator> ssc::Streams::getStreams()
{
    return mSession ? std::make_unique<MediaSubsessionIterator>(*mSession) : nullptr;
}
