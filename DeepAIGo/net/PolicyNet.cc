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

#include "PolicyNet.h"

#include <fstream>

namespace DeepAIGo
{
	PolicyNet::PolicyNet()
		: process_({ ProcessorType::STONE_COLOR, ProcessorType::ONES, ProcessorType::TURNS_SINCE, ProcessorType::LIBERTIES,
			ProcessorType::CAPTURE_SIZE, ProcessorType::SELF_ATARI_SIZE, ProcessorType::LIBERTIES_AFTER_MOVE, ProcessorType::SENSIBLENESS,
			ProcessorType::ZEROS })
	{
		using namespace mxnet::cpp;

		Symbol data = Symbol::Variable("data");
		Symbol data_label = Symbol::Variable("softmax_label");

		std::vector<mxnet::cpp::Symbol> layers;
		layers.emplace_back(ConvFactory(data, 128, Shape(5, 5), Shape(1, 1), Shape(2, 2), "0"));

		for (int i = 1; i < 12; i++)
		{
			layers.emplace_back(
				ConvFactory(layers.back(), 128, Shape(3, 3), Shape(1, 1), Shape(1, 1), std::to_string(i)));
		}
	
		Symbol conv_w("convolution12_weight"), conv_b("convolution12_bias");

		layers.push_back(Convolution("convolution12", layers.back(),
			conv_w, conv_b, Shape(1, 1),
			1, Shape(1, 1), Shape(1, 1), Shape(0, 0)));

		auto flatten = Flatten(layers.back());

		net_ = SoftmaxOutput("softmax", flatten, data_label);
	}

	PolicyNet::~PolicyNet()
	{
		if (exec_)
			delete exec_;
	}

	std::vector<ActionProb> PolicyNet::EvalState(const Board& board, int symmetric)
	{
		using namespace mxnet::cpp;

		auto input = make_input(board, symmetric);

		args_["data"].SyncCopyFromCPU(input.data(), input.num_elements());

		exec_->Forward(false);
		NDArray::WaitAll();
		
		Tensor output { boost::extents[1][BOARD_SIZE][BOARD_SIZE] };
		exec_->outputs[0].SyncCopyToCPU(output.data(), BOARD_SIZE2);

		auto converted = make_output(output, symmetric);

		std::vector<ActionProb> ret;

		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			for (size_t y = 0; y < BOARD_SIZE; ++y)
			{
				auto pt = Point(x, y);

				if (board.IsValidMove(pt) && !board.IsTrueEye(pt, board.GetCurrentPlayer()))
				{
					ret.emplace_back(std::make_tuple(pt, converted[0][y][x]));
				}
			}
		}

		return ret;
	}

	void PolicyNet::InitNetwork()
	{
		using namespace mxnet::cpp;

		args_["data"] = NDArray(Shape(1, process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE), Context::cpu());
		args_["softmax_label"] = NDArray(Shape(1), Context::cpu());
		net_.InferArgsMap(Context::cpu(), &args_, args_);

		exec_ = net_.SimpleBind(Context::cpu(), args_);
		LoadCheckpoint("policy.params", exec_);
	}

	Tensor PolicyNet::make_input(const Board& board, int symmetric)
	{
		Tensor ret { boost::extents[process_.GetOutputDim()][BOARD_SIZE][BOARD_SIZE] };

		auto origin = process_.StateToTensor(board);

		switch (symmetric)
		{
		case 0: ret = origin; break;
		case 1: ret = Symmetrics::Rot90(origin); break;
		case 2: ret = Symmetrics::Rot180(origin); break;
		case 3: ret = Symmetrics::Rot270(origin); break;
		case 4: ret = Symmetrics::FlipUD(origin); break;
		case 5: ret = Symmetrics::FlipLR(origin); break;
		case 6: ret = Symmetrics::Diag1(origin); break;
		case 7: ret = Symmetrics::Diag2(origin); break;
		default: throw std::runtime_error("Invalid symmetric");
		}

		return ret;
	}

	Tensor PolicyNet::make_output(const Tensor& output, int symmetric)
	{
		Tensor ret { boost::extents[1][BOARD_SIZE][BOARD_SIZE] };

		switch (symmetric)
		{
		case 0: ret = output; break;
		case 1: ret = Symmetrics::DRot90(output); break;
		case 2: ret = Symmetrics::DRot180(output); break;
		case 3: ret = Symmetrics::DRot270(output); break;
		case 4: ret = Symmetrics::DFlipUD(output); break;
		case 5: ret = Symmetrics::DFlipLR(output); break;
		case 6: ret = Symmetrics::DDiag1(output); break;
		case 7: ret = Symmetrics::DDiag2(output); break;
		default: throw std::runtime_error("Invalid symmetric");
		}

		return ret;
	}
}
