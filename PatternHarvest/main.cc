#include <boost/program_options.hpp>
#include <iostream>

namespace bpo = boost::program_options;

int main(int argc, char** argv)
{
    bpo::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce a help screen")
        ("folder,f", bpo::value<std::string>(), "sgf folder")
        ("output,o", bpo::value<std::string>(), "output file")
        ;

    bpo::variables_map vm;

    try
    {
        bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            std::cout << "[Usage]: PatternHarvest [params]" << std::endl
            << desc;

            return 0;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
    }

    return 0;
}