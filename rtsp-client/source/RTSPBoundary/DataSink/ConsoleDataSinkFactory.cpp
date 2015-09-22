#include "stdafx.h"
#include "ConsoleDataSinkFactory.h"
#include "ConsoleDataSink.h"
#include "RTSPBoundary/Streams/Stream.h"

MediaSink * ssc::ConsoleDataSinkFactory::createDataSink(UsageEnvironment &env, Stream &stream) const
{
	return new ConsoleDataSink(env, *stream.getSubSession());
}
