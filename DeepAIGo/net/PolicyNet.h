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

#include "helper.h"
#include "../preprocess/Preprocess.h"

#include <tuple>
#include <memory>

namespace DeepAIGo
{
	//! 행동 및 확률
	using ActionProb = std::tuple<Point, float>;

	//! Policy Network
	class PolicyNet
	{
	public:
		using Ptr = std::shared_ptr<PolicyNet>;

	public:
		PolicyNet();
		virtual ~PolicyNet();

		/** Policy Network를 연산합니다.
		 * @return 연산 결과
		 * @param board 바둑판 상태
		 * @param symmetric 바둑판 변형 타입
		 **/
		std::vector<ActionProb> EvalState(const Board& board, int symmetric = 0);

		//! 신경망을 초기화 합니다.
		virtual void InitNetwork();

	private:
		/** 입력 데이터를 만듭니다.
		 * @return 입력 데이터
		 * @param board 바둑판 상태
		 * @param symmetric 바둑판 변형 타입
		 **/
		Tensor make_input(const Board& board, int symmetric);
		/** 출력 데이터를 만듭니다.
		 * @return 출력 데이터
		 * @param output 신경망 출력 값
		 * @param symmetric 바둑판 변형 타입
		 **/
		Tensor make_output(const Tensor& output, int symmetric);

	protected:
		//! 신경망
		mxnet::cpp::Symbol net_;
		//! 전처리기
		Preprocess process_;

		//! Executor
		mxnet::cpp::Executor* exec_;
		//! 신경망 파라메터 목록
		std::map<std::string, mxnet::cpp::NDArray> args_;
	};
}
