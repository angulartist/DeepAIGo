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

		std::vector<ActionProb> EvalState(const Board& board, int symmetric = 0);

		virtual void InitNetwork();

	private:
		boost::multi_array<mx_float, 4> make_input(const Board& board, int symmetric);
		boost::multi_array<mx_float, 2> convert_output(const Tensor& output, int symmetric);

	protected:
		mxnet::cpp::Symbol net_;
		Preprocess process_;

		mxnet::cpp::Executor* exec_;
		std::map<std::string, mxnet::cpp::NDArray> args_;
	};
}
