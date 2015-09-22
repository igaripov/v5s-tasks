#pragma once

namespace ssc {
	class DescribeCallback;
	class RTSPConnection;
    class SetupCallback;
    class PlayCallback;
	class RTSPCommand;
	class Streams;
    class Stream;

	class RTSPCommandFactory
	{
	public:
		static RTSPCommand createDescribe(RTSPConnection &connection, DescribeCallback &callback);
		static RTSPCommand createSetup(RTSPConnection &connection, Stream &stream, SetupCallback &callback);
		static RTSPCommand createPlay(RTSPConnection &connection, PlayCallback &callback);
	private:
		RTSPCommandFactory();
		~RTSPCommandFactory();
	};
} // ~namespace ssc
