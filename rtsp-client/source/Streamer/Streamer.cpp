#include "stdafx.h"
#include "Streamer.h"
#include "Executor/Describer.h"
#include "RTSPBoundary/Connection/RTSPConnection.h"
#include "RTSPBoundary/DataSink/ADataSinkFactory.h"

ssc::Streamer::Streamer(ADataSinkFactory &sinkFactory)
	: mDataSinkFactory(sinkFactory)
{

}

ssc::Streamer::~Streamer() {

}

void ssc::Streamer::addURL(const char *url)
{
    std::shared_ptr<RTSPConnection> connection(mRTSPController.createConnect(url));
    auto describer = std::make_shared<Describer>((IExecutorCallback&)*this, *connection);
    mExecutors.emplace(connection, describer);
    describer->execute();
}

void ssc::Streamer::startStreaming()
{
	mRTSPController.executionLoop();
}

void ssc::Streamer::executorDone(RTSPConnection &connection)
{
	std::shared_ptr<RTSPConnection> connectionToDelete(&connection, [](void *) {}); // null deleter
	mExecutors.erase(connectionToDelete);

	if (mExecutors.empty())
	{
        mRTSPController.stop();
	}
}

void ssc::Streamer::executorChanged(RTSPConnection &connection, AExecutor *newExecutor)
{
    std::shared_ptr<RTSPConnection> refConnection(&connection, [](void *) { }); // null deleter
    mExecutors[refConnection] = std::shared_ptr<AExecutor>(newExecutor);
    newExecutor->execute();
}

MediaSink * ssc::Streamer::createDataSink(Stream &stream)
{
    return mDataSinkFactory.createDataSink(mRTSPController.getEnv(), stream);
}

ssc::RTSPController & ssc::Streamer::getRTSPController()
{
    return mRTSPController;
}
