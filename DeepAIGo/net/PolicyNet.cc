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
		layers.push_back(ConvFactory(data, 128, Shape(5, 5), Shape(1, 1), Shape(2, 2), "0"));

		for (int i = 1; i < 7; i++)
		{
			layers.push_back(
				ConvFactory(layers.back(), 128, Shape(3, 3), Shape(1, 1), Shape(1, 1), std::to_string(i)));
		}

		Symbol conv_w("convolution7_weight"), conv_b("convolution7_bias");

		layers.push_back(Convolution("convolution7", layers.back(),
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

	std::vector<ActionProb> PolicyNet::EvalState(const Board& board)
	{
		using namespace mxnet::cpp;

		auto input = process_.StateToTensor(board);

		args_["data"].SyncCopyFromCPU(input.data(), input.num_elements());

		NDArray::WaitAll();

		exec_->Forward(false);

		using T = boost::multi_array<mx_float, 2>;
		T output{ boost::extents[1][BOARD_SIZE * BOARD_SIZE] };

		exec_->outputs[0].SyncCopyToCPU(output.data(), BOARD_SIZE2);
		NDArray::WaitAll();

		boost::array<T::index, 2> dims = { {BOARD_SIZE, BOARD_SIZE} };
		output.reshape(dims);
		std::vector<ActionProb> ret;

		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			for (size_t y = 0; y < BOARD_SIZE; ++y)
			{
				auto pt = Point(x, y);

				if (board.IsValidMove(pt) && !board.IsTrueEye(pt, board.GetCurrentPlayer()))
				{
					ret.push_back(std::make_tuple(pt, output[y][x]));
				}
			}
		}

		return ret;
	}

	void PolicyNet::InitNetwork()
	{
		using namespace mxnet::cpp;

		args_["data"] = NDArray(Shape(1, process_.GetOutputDim(), 19, 19), Context::cpu());
		args_["softmax_label"] = NDArray(Shape(1), Context::cpu());
		net_.InferArgsMap(Context::cpu(), &args_, args_);

		exec_ = net_.SimpleBind(Context::cpu(), args_);
		LoadCheckpoint("policy.params", exec_);
	}
}
