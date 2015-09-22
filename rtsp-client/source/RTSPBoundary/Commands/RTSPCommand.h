#pragma once

namespace ssc {
	class RTSPCommand
	{
	public:
        RTSPCommand() = default;
        RTSPCommand(const RTSPCommand &) = default;
        RTSPCommand(RTSPCommand &&) = default;
		RTSPCommand(std::function<void()> &command);
		void execute();
	private:
		std::function<void()> mCommand;
	};

} // ~namespace ssc
