/* MIT License

Copyright (c) 2017 ParkJunYeong(https://github.com/ParkJunYeong)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

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
