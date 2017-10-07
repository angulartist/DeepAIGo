#pragma once

#include "../board/Board.h"
#include "GtpEngine.h"

namespace DeepAIGo
{
	class AIGoGtp : public GtpEngine
	{
	public:
		AIGoGtp();

		virtual std::string Process(const GtpCmd& command);

	private:
		Board board_;
	};
}
