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

#include <gtp/AIGoGtp.h>

#include <iostream>
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;
using namespace DeepAIGo;

int main(int argc, char** argv)
{
	bpo::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce a help screen")
		("gtp", "run aigo with gtp engine")
		;

	bpo::variables_map vm;

	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{
		std::cout << "[Usage]: AIGoGtp [params]" << std::endl
			<< desc;

		return 0;
	}
	else if (vm.count("gtp"))
	{
		AIGoGtp gtp;

		gtp.Run(std::cin, std::cout);
	}
	else
	{
		MCTSEngine engine;
		Board board;

		while (!board.IsEnded())
		{
			auto pt = engine.GenMove(board);
			board.DoMove(pt);
			engine.DoMove(pt);
			board.ShowBoard();
			std::cout << std::endl;
		}
	}

	return 0;
}
