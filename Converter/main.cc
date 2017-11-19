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