#include <gtp/AIGoGtp.h>

#include <net/PolicyNet.h>

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
			std::cout << "[Usage]: DeepAIGo [params]" << std::endl
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
			PolicyNet net;
			net.InitNetwork();

			Board board;

			while (!board.IsEnded())
			{
				auto output = net.EvalState(board);
				auto maxe = std::max_element(output.begin(), output.end(), [](const ActionProb& lhs, const ActionProb& rhs) {
					return std::get<1>(lhs) < std::get<1>(rhs);
				});

				board.DoMove(std::get<0>(maxe[0]));
				board.ShowBoard();
				std::cout << std::endl;

				std::cin.get();
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
