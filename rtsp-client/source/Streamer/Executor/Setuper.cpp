#include "stdafx.h"
#include "Setuper.h"
#include "Player.h"
#include "Streamer/Streamer.h"
#include "RTSPBoundary/RTSPController.h"
#include "RTSPBoundary/Streams/Stream.h"
#include "RTSPBoundary/Streams/Streams.h"
#include "RTSPBoundary/Connection/RTSPConnection.h"
#include "RTSPBoundary/Commands/RTSPCommandFactory.h"

ssc::Setuper::Setuper(IExecutorCallback &owner, RTSPConnection &connection)
    : AExecutor(owner, connection)
{
}

void ssc::Setuper::execute()
{
    initCommands();
    postCommand();
}

void ssc::Setuper::failed(RTSPConnection *connection, const char *error)
{
    if (!postCommand())
        mOwner.executorDone(mConnection);
}

void ssc::Setuper::setupStream(Stream &stream)
{
    sinkInitializedStream(stream);
    if (!postCommand())
        playStreams();
}

void ssc::Setuper::setupCompleted(Streams *streams)
{
}

void ssc::Setuper::initCommands()
{
    Streams &streams = mConnection.getStreams();
    for (std::size_t i = 0; i < streams.size(); ++i)
    {
        auto &stream = streams.get(i);
        RTSPCommand&& command = RTSPCommandFactory::createSetup(mConnection, stream, *this);
        mCommands.push_back(command);
    }
}

bool ssc::Setuper::postCommand()
{
    if (mCommands.empty())
        return false;
    RTSPController &controller = mOwner.getRTSPController();
    RTSPCommand command = mCommands.front();
    mCommands.pop_front();
    controller.postCommand(command);
    return true;
}

void ssc::Setuper::sinkInitializedStream(Stream &stream)
{
    MediaSink *sink = mOwner.createDataSink(stream);
    if (!sink)
        return;

    stream.getSubSession()->sink = sink;
}

void ssc::Setuper::playStreams()
{
    auto player = std::make_unique<Player>(mOwner, mConnection);
    mOwner.executorChanged(mConnection, player.release());
}
