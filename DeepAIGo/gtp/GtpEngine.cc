#include "GtpEngine.h"

#include <boost/algorithm/string.hpp>
#include <sstream>
#include <map>

namespace DeepAIGo
{
	namespace
	{
		std::map<std::string, GtpCmdType> cmd_str = {
			{ "quit", GtpCmdType::QUIT },
			{ "protocol_version", GtpCmdType::PROTOCOL_VERSION },
			{ "name", GtpCmdType::NAME },
			{ "version", GtpCmdType::VERSION },
			{ "clear_board", GtpCmdType::CLEAR_BOARD },
			{ "showboard", GtpCmdType::SHOW_BOARD },
			{ "play", GtpCmdType::PLAY },
			{ "genmove", GtpCmdType::GENMOVE },
		};
	}

	GtpEngine::GtpEngine(const std::string& name, const std::string& version)
		: name_(name), version_(version), is_run_(false), cmd_idx_(1)
	{
	}

	bool GtpEngine::IsRunning() const
	{
		return is_run_;
	}

	void GtpEngine::Run(std::istream& stream, std::ostream& outstream)
	{
		is_run_ = true;

		while (is_run_)
		{
			std::string lower;
			std::getline(stream, lower);
			boost::to_lower(lower);

			std::vector<std::string> tokens;
			boost::split(tokens, lower, boost::is_any_of(" "));

			auto cmd = parse_command(tokens);
			std::string response = "=" + std::to_string(cmd_idx_) + " ";

			switch (cmd.command)
			{
			case GtpCmdType::QUIT:
				is_run_ = false;
				break;

			case GtpCmdType::NAME:
				response.append(name_);
				break;

			case GtpCmdType::VERSION:
				response.append(version_);
				break;

			case GtpCmdType::PROTOCOL_VERSION:
				response.append(std::to_string(2));
				break;

			default:
				response.append(Process(cmd));
			}

			++cmd_idx_;
			outstream << response << std::endl << std::endl;
		}
	}
	
	StoneType GtpEngine::parse_color(const std::string& arg)
	{
		if (arg == "b")
			return StoneType::BLACK;
		else
			return StoneType::WHITE;
	}

	Point GtpEngine::parse_coord(const std::string& arg)
	{
		static const std::string coord = "abcdefghijkmnopqrstuvwxyz";

		return Point(coord.find_first_of(arg[0]), atoi(arg.substr(1).c_str()) - 1);
	}

	std::string GtpEngine::coord_to_str(const Point& pt)
	{
		static const std::string coord = "ABCDEFGHIJKMNOPQRSTUVWXYZ";

		std::stringstream ss;
		ss << (char)coord[pt.X];
		ss << pt.Y + 1;
		
		return ss.str();
	}

	GtpCmd GtpEngine::parse_command(const std::vector<std::string>& tokens)
	{
		if (tokens.size() == 0)
			return GtpCmd(GtpCmdType::QUIT);

		auto it = cmd_str.find(tokens[0]);

		GtpCmd cmd;

		if (it != cmd_str.end())
			cmd.command = it->second;
		else
			cmd.command = GtpCmdType::UNKNOWN;

		if (tokens.size() > 1)
			cmd.args.insert(cmd.args.begin(), tokens.begin() + 1, tokens.end());

		return cmd;
	}
}
