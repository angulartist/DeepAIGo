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

#include "SgfParser.h"

#include <boost/algorithm/string.hpp>
#include <regex>
#include <sstream>
#include <fstream>
#include <iostream>

namespace DeepAIGo
{
	Board SgfParser::Parse(const std::string& sgf)
	{
		Board board;

		std::vector<std::string> tokens;
		boost::split(tokens, sgf, boost::is_any_of(";"));

		if (tokens.size() < 3)
			throw std::runtime_error("Invalid SGF file");

		std::stringstream ss(tokens[1]);
		std::vector<std::string> lines;

		std::string line;
		while (!ss.eof())
		{
			std::string s;
			std::getline(ss, s);

			if (s == "") continue;

			if (s[0] != '[')
			{
				lines.push_back(line);
				line = s;
			}
			else
			{
				line.append(s);
			}
		}

		if (line != "")
			lines.push_back(line);

		for (auto& l : lines)
		{
			if (l == "")
				continue;

			std::string cmd = l.substr(0, 2);

			boost::replace_all(l, "]", "");
			std::vector<std::string> argument;
			std::string arg = l.substr(2);

			boost::split(argument, arg, boost::is_any_of("["));
			argument.erase(argument.begin());

			if (cmd == "AB")
			{
				for (auto& arg : argument)
				{
					board.PlaceHandicap(Point(arg[0] - 'a', 18 - (arg[1] - 'a')));
				}
			}
		}

		std::vector<std::string> moves(tokens.begin() + 2, tokens.end());
		int turns = 0;

		for (auto& m : moves)
		{
			turns += 1;
			std::string cmd = m.substr(0, 1);
			std::string coord = m.substr(2, 2);

			Point pt(coord[0] - 'a', 18 - (coord[1] - 'a'));
			if (!(pt.X >= 0 && pt.Y >= 0 && pt.X < BOARD_SIZE && pt.Y < BOARD_SIZE))
				pt = Pass;

			if (cmd == "B")
				board.DoMove(pt, StoneType::BLACK);
			else
				board.DoMove(pt, StoneType::WHITE);
		}

		return board;
	}

	Board SgfParser::LoadFromFile(const std::string& filename)
	{
		std::ifstream file(filename);

		if (file.is_open())
		{
			std::string sgf((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			return Parse(sgf);
		}
		else
		{
			throw std::runtime_error("Cannot open file");
		}
	}
}
