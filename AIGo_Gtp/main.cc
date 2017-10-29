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

	try
	{
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
	}
	catch (std::exception& e)
	{
		std::cerr << "[Error] " << e.what();
		return -1;
	}

	return 0;
}
