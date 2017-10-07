#pragma once

#include "helper.h"
#include "../preprocess/Preprocess.h"

#include <tuple>

namespace DeepAIGo
{
	using ActionProb = std::tuple<Point, float>;

	class PolicyNet
	{
	public:
		PolicyNet();
		virtual ~PolicyNet();

		std::vector<ActionProb> EvalState(const Board& board);

		virtual void InitNetwork();

	protected:
		mxnet::cpp::Symbol net_;
		Preprocess process_;

		mxnet::cpp::Executor* exec_;
		std::map<std::string, mxnet::cpp::NDArray> args_;
	};
}
