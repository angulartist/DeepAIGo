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
		size_t symmetric_idx(const Point& pt, int symmetric) const;

	protected:
		mxnet::cpp::Symbol net_;	//! 신경망
		Preprocess process_;		//! 전처리기

		mxnet::cpp::Executor* exec_;					  //! Executor
		std::map<std::string, mxnet::cpp::NDArray> args_; //! 신경망 파라메터 목록
	};
}
