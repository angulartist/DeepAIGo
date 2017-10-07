#include "PolicyTrainer.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace bpo = boost::program_options;

int main(int argc, char** argv)
{
	bpo::options_description desc;
	desc.add_options()
		("help,h", "produce help screen")
		("train", bpo::value<std::string>(), "train dataset")
		("test", bpo::value<std::string>(), "test dataset")
		("batch_size", bpo::value<int>(), "mini batch size")
		("epoch", bpo::value<int>(), "training epoch")
		;

	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{
		std::cout << "Usage: SLPolicy [parms]" << std::endl;
		std::cout << desc;
	}
	else
	{
		PolicyTrainer trainer(vm["train"].as<std::string>(), vm["test"].as<std::string>(), 128, 10);

		trainer.Train();
	}

	return 0;
}