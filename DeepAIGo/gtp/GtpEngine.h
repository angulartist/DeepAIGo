#pragma once

#include "../board/Board.h"

#include <boost/noncopyable.hpp>
#include <vector>
#include <iostream>

namespace DeepAIGo
{
	enum class GtpCmdType
	{
		QUIT,
		PROTOCOL_VERSION,
		NAME,
		VERSION,
		CLEAR_BOARD,
		PLAY,
		GENMOVE,
		UNKNOWN
	};

	struct GtpCmd
	{
		GtpCmdType command;
		std::vector<std::string> args;

		GtpCmd() : command(GtpCmdType::UNKNOWN) { }
		GtpCmd(GtpCmdType cmd) : command(cmd) { }
	};

	class GtpEngine : public boost::noncopyable
	{
	public:
		GtpEngine(const std::string& name, const std::string& version);
		virtual ~GtpEngine() = default;

		bool IsRunning() const;
		void Run(std::istream& stream, std::ostream& outstream);

		virtual std::string Process(const GtpCmd& command) = 0;

	protected:
		StoneType parse_color(const std::string& arg);
		GtpCmd parse_command(const std::vector<std::string>& tokens);

	private:
		std::string name_;
		std::string version_;
		bool is_run_;
		int cmd_idx_;
	};
}
