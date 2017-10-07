#include "AIGoGtp.h"

namespace DeepAIGo
{
	AIGoGtp::AIGoGtp()
		: GtpEngine("DeepAIGo", "3"), board_()
	{
	}

	std::string AIGoGtp::Process(const GtpCmd& command)
	{
		if (command.command == GtpCmdType::GENMOVE)
		{
			return "resign";
		}

		return "";
	}
}
