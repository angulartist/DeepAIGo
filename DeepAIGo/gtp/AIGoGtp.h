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
