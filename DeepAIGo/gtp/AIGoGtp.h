#pragma once

#include "../board/Board.h"
#include "../net/PolicyNet.h"
#include "../mcts/MCTSEngine.h"
#include "GtpEngine.h"

namespace DeepAIGo
{
	//! AIGo의 GTP 엔진
	class AIGoGtp : public GtpEngine
	{
	public:
		AIGoGtp();

		/** 명령어를 처리합니다.
		 * @return 처리 결과
		 * @param command GTP 명령
		 **/
		virtual std::string Process(const GtpCmd& command);

	private:
		//! 바둑판
		Board board_;

		//! MCTS 엔진
		MCTSEngine engine_;
	};
}
