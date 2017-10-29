#include "AIGoGtp.h"

#include <random>
#include <ctime>

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
			auto pt = engine_.GenMove(board_);
			board_.DoMove(pt);

			return coord_to_str(pt);
		}
		else if (command.command == GtpCmdType::PLAY)
		{
			auto color = parse_color(command.args[0]);
			auto coord = parse_coord(command.args[1]);

			board_.DoMove(coord, color);
			engine_.DoMove(coord);
		}
		else if (command.command == GtpCmdType::CLEAR_BOARD)
		{
			board_.Clear();
		}
		else if (command.command == GtpCmdType::SHOW_BOARD)
		{
			return board_.ToString();
		}

		return "";
	}
}
