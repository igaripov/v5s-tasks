#pragma once

namespace ssc {
	class IRTSPConnectionCallbacks;
	class RTSPConnection;

	class ARTSPCommandCallback
	{
	public:
		virtual ~ARTSPCommandCallback();

		virtual void handle(IRTSPConnectionCallbacks &command) = 0;

		virtual void failed(RTSPConnection *connection, const char *error) = 0;
	private:

	};
} // ~namespace ssc