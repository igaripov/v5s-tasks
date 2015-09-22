#include "stdafx.h"
#include "RTSPCommand.h"

ssc::RTSPCommand::RTSPCommand(std::function<void()> &command)
	: mCommand(command)
{

}

void ssc::RTSPCommand::execute()
{
	mCommand();
}
