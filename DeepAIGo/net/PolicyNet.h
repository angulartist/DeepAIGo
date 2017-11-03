#pragma once

#include "helper.h"
#include "../preprocess/Preprocess.h"

#include <tuple>
#include <memory>

namespace DeepAIGo
{
	using ActionProb = std::tuple<Point, float>;

	class PolicyNet
	{
	public:
		using Ptr = std::shared_ptr<PolicyNet>;

	public:
		PolicyNet();
		virtual ~PolicyNet();

		std::vector<ActionProb> EvalState(const Board& board, int symmetric = 0);

		virtual void InitNetwork();

	private:
		Tensor make_input(const Board& board, int symmetric);
		size_t symmetric_idx(const Point& pt, int symmetric) const;

	protected:
		mxnet::cpp::Symbol net_;
		Preprocess process_;

		mxnet::cpp::Executor* exec_;
		std::map<std::string, mxnet::cpp::NDArray> args_;
	};
}
