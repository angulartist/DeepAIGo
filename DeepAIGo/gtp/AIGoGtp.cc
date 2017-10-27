#include "AIGoGtp.h"

#include <random>
#include <ctime>

namespace DeepAIGo
{
	AIGoGtp::AIGoGtp()
		: GtpEngine("DeepAIGo", "3"), board_(), net_()
	{
		net_.InitNetwork();
	}

	std::string AIGoGtp::Process(const GtpCmd& command)
	{
		if (command.command == GtpCmdType::GENMOVE)
		{
			std::uniform_int_distribution<int> dist(0, 7);
			std::mt19937 engine((unsigned int)time(NULL));

			auto output = net_.EvalState(board_, dist(engine));
			auto maxe = std::max_element(output.begin(), output.end(), [](const ActionProb& lhs, const ActionProb& rhs) {
				return std::get<1>(lhs) < std::get<1>(rhs);
			});

			board_.DoMove(std::get<0>(maxe[0]));
			return coord_to_str(std::get<0>(maxe[0]));
		}
		else if (command.command == GtpCmdType::PLAY)
		{
			auto color = parse_color(command.args[0]);
			auto coord = parse_coord(command.args[1]);

			board_.DoMove(coord, color);
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
