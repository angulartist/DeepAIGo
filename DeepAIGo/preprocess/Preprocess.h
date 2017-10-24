#pragma once

#include "../board/Board.h"

#include <algorithm>
#include <functional>
#include <vector>

#include "Symmetrics.h"

namespace DeepAIGo
{
	enum class ProcessorType
	{
		STONE_COLOR,
		ONES,
		TURNS_SINCE,
		LIBERTIES,
		CAPTURE_SIZE,
		SELF_ATARI_SIZE,
		LIBERTIES_AFTER_MOVE,
		SENSIBLENESS,
		ZEROS,
		PLAYER_COLOR
	};

	struct Processor
	{
		size_t output_dim_;
		std::function<Tensor(const Board&)> impl_;

		Processor() { }
		Processor(size_t output_dim, std::function<Tensor(const Board&)> impl) : impl_(impl), output_dim_(output_dim) { }
	};

	class Preprocess
	{
	public:
		Preprocess(const std::vector<ProcessorType>& process);

		size_t GetOutputDim() const;
		Tensor StateToTensor(const Board& board);

	private:
		std::vector<Processor> process_;
		size_t output_dim_;
	};
}
