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

#include "Preprocess.h"

#include <map>

namespace DeepAIGo
{
	namespace
	{
		std::map<ProcessorType, Processor> processors = {
			{ ProcessorType::STONE_COLOR, Processor(3, [](const Board& board) {
			Tensor ret{ boost::extents[3][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			for (size_t x = 0; x < BOARD_SIZE; ++x)
			{
				for (size_t y = 0; y < BOARD_SIZE; ++y)
				{
					auto pt = Point(x, y);

					if (board.GetStoneColor(pt) == StoneType::EMPTY)
					{
						ret[0][y][x] = 1;
					}
					else if (board.GetStoneColor(pt) == board.GetCurrentPlayer())
					{
						ret[1][y][x] = 1;
					}
					else
					{
						ret[2][y][x] = 1;
					}
				}
			}

			return ret;
		}) },

		{ ProcessorType::ONES, Processor(1, [](const Board& board) {
			Tensor ret{ boost::extents[1][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 1);

			return ret;
		}) },

		{ ProcessorType::TURNS_SINCE, Processor(8, [](const Board& board) {
			Tensor ret{ boost::extents[8][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			if (board.GetHistory().size() == 0)
				return ret;
			for (int i = 0; i < std::min((int)board.GetHistory().size(), 8); ++i)
			{
				int idx = std::max((int)board.GetHistory().size() - (i + 1), 0);
				auto& history = board.GetHistory()[idx];

				if (board.IsOnBoard(history))
					ret[i][history.Y][history.X] = 1;
			}

			return ret;
		}) },

		{ ProcessorType::LIBERTIES, Processor(8, [](const Board& board) {
			Tensor ret{ boost::extents[8][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			for (size_t x = 0; x < BOARD_SIZE; ++x)
			{
				for (size_t y = 0; y < BOARD_SIZE; ++y)
				{
					auto pt = Point(x, y);

					if (board.GetLibertyCount(pt) > 0)
						ret[std::min(board.GetLibertyCount(pt) - 1, 7)][y][x] = 1;
				}
			}

			return ret;
		}) },

		{ ProcessorType::CAPTURE_SIZE, Processor(8, [](const Board& board) {
			Tensor ret{ boost::extents[8][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			for (auto& m : board.GetLegalMoves())
			{
				int captured = 0;

				for (auto& n : board.GetNeighborGroups(m))
				{
					if (n.size() == 0) continue;

					if (board.GetLibertyCount(*n.begin()) == 1 && board.GetStoneColor(*n.begin()) == board.GetOpponentPlayer())
						captured += n.size();
				}

				if (captured > 0)
				{
					ret[std::min(captured - 1, 7)][m.Y][m.X] = 1;
				}
			}

			return ret;
		}) },

		{ ProcessorType::SELF_ATARI_SIZE, Processor(8, [](const Board& board) {
			Tensor ret{ boost::extents[8][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			for (auto& m : board.GetLegalMoves())
			{
				auto liberties = board.GetLiberties(m);

				PointSet groups, captured;
				groups.insert(m);

				for (auto& n : board.GetNeighborGroups(m))
				{
					if (n.size() == 0) continue;

					auto& g = board.GetGroup(*n.begin());

					if (board.GetStoneColor(*n.begin()) == board.GetCurrentPlayer())
					{
						auto& li = board.GetLiberties(*n.begin());
						liberties.insert(li.begin(), li.end());
						groups.insert(g.begin(), g.end());
					}
					else if (board.GetLibertyCount(*n.begin()) == 1)
					{
						captured.insert(g.begin(), g.end());
					}
				}

				if (captured.size() > 0)
				{
					for (auto& g : groups)
					{
						for (auto& n : board.GetNeighbor(g))
						{
							if (std::find(captured.begin(), captured.end(), n) != captured.end())
								liberties.insert(n);
						}
					}
				}

				auto it = std::find(liberties.begin(), liberties.end(), m);
				if (it != liberties.end())
					liberties.erase(it);

				if (liberties.size() == 1)
					if (groups.size() > 0)
						ret[std::min((int)groups.size() - 1, 7)][m.Y][m.X] = 1;
			}

			return ret;
		}) },

		{ ProcessorType::LIBERTIES_AFTER_MOVE, Processor(8, [](const Board& board) {
			Tensor ret{ boost::extents[8][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			for (auto& m : board.GetLegalMoves())
			{
				auto liberties = board.GetLiberties(m);

				PointArr groups, captured;
				groups.push_back(m);

				for (auto& n : board.GetNeighborGroups(m))
				{
					if (n.size() == 0) continue;

					auto& g = board.GetGroup(*n.begin());

					if (board.GetStoneColor(*n.begin()) == board.GetCurrentPlayer())
					{
						auto& li = board.GetLiberties(*n.begin());
						liberties.insert(li.begin(), li.end());
						groups.insert(groups.end(), g.begin(), g.end());
					}
					else if (board.GetLibertyCount(*n.begin()) == 1)
					{
						captured.insert(captured.end(), g.begin(), g.end());
					}
				}

				if (captured.size() > 0)
				{
					for (auto& g : groups)
					{
						for (auto& n : board.GetNeighbor(g))
						{
							if (std::find(captured.begin(), captured.end(), n) != captured.end())
								liberties.insert(n);
						}
					}
				}

				auto it = std::find(liberties.begin(), liberties.end(), m);
				if (it != liberties.end())
					liberties.erase(it);

				if (liberties.size() > 0)
					ret[std::min((int)liberties.size() - 1, 7)][m.Y][m.X] = 1;
			}

			return ret;
		}) },

		{ ProcessorType::SENSIBLENESS, Processor(1, [](const Board& board) {
			Tensor ret{ boost::extents[1][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			for (auto& m : board.GetLegalMoves())
			{
				ret[0][m.Y][m.X] = 1;
			}

			return ret;
		}) },

		{ ProcessorType::ZEROS, Processor(1, [](const Board& board) {
			Tensor ret{ boost::extents[1][BOARD_SIZE][BOARD_SIZE] };
			std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

			return ret;
		}) },

		{ ProcessorType::PLAYER_COLOR, Processor(1, [](const Board& board) {
			Tensor ret{ boost::extents[1][BOARD_SIZE][BOARD_SIZE] };

			std::fill(ret.origin(), ret.origin() + ret.num_elements(), (board.GetCurrentPlayer() == StoneType::BLACK) ? 1 : 0);

			return ret;
		}) },
		};
	}

	Preprocess::Preprocess(const std::vector<ProcessorType>& process)
	{
		output_dim_ = 0;

		for (auto& p : process)
		{
			process_.push_back(processors[p]);
			output_dim_ += processors[p].output_dim_;
		}
	}

	size_t Preprocess::GetOutputDim() const
	{
		return output_dim_;
	}

	Tensor Preprocess::StateToTensor(const Board& board)
	{
		Tensor ret{ boost::extents[output_dim_][BOARD_SIZE][BOARD_SIZE] };

		size_t idx = 0;
		for (auto& p : process_)
		{
			auto t = p.impl_(board);

			for (size_t i = 0; i < p.output_dim_; ++i)
			{
				ret[idx] = t[i];

				++idx;
			}
		}

		return ret;
	}
}
