#pragma once

#include "../board/Board.h"
#include "../net/PolicyNet.h"
#include "../mcts/MCTSEngine.h"
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

		MCTSEngine engine_;
	};
}
