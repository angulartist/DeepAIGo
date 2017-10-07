#include "Converter.h"

#include <Config.h>

#include <boost/program_options.hpp>
#include <iostream>

namespace bpo = boost::program_options;

int main(int argc, char** argv)
{
	bpo::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce a help screen")
		("version,v", "print supported DeepAIGo version")
		("folder,f", bpo::value<std::string>(), "folder of sgf files")
		("output,o", bpo::value<std::string>(), "saved hdf5 file name")
		;

	try
	{
		bpo::variables_map vm;
		bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help"))
		{
			std::cout << "Usage: Converter [params]" << std::endl;
			std::cout << desc;
		}
		else if (vm.count("version"))
		{
			std::cout << "DeepAIGo version: " << DEEPAIGO_VER_STR << std::endl;
		}
		else
		{
			auto folder = vm["folder"].as<std::string>();
			auto output = vm["output"].as<std::string>();

			Converter conv;
			conv.Convert(folder, output);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what();
	}

	return 0;
}