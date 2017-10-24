#include "PolicyNet.h"

#include <fstream>

namespace DeepAIGo
{
	PolicyNet::PolicyNet(int symmetrics)
		: process_({ ProcessorType::STONE_COLOR, ProcessorType::ONES, ProcessorType::TURNS_SINCE, ProcessorType::LIBERTIES,
			ProcessorType::CAPTURE_SIZE, ProcessorType::SELF_ATARI_SIZE, ProcessorType::LIBERTIES_AFTER_MOVE, ProcessorType::SENSIBLENESS,
			ProcessorType::ZEROS }), symmetrics_(symmetrics)
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

		auto input = make_input(board);

		args_["data"].SyncCopyFromCPU(input.data(), input.num_elements());

		NDArray::WaitAll();

		exec_->Forward(false);

		Tensor outputs{ boost::extents[1][1][symmetrics_ * BOARD_SIZE2] };

		exec_->outputs[0].SyncCopyToCPU(outputs.data(), symmetrics_ * BOARD_SIZE2);
		NDArray::WaitAll();

		boost::array<Tensor::index, 3> dims = { {symmetrics_, BOARD_SIZE, BOARD_SIZE} };
		outputs.reshape(dims);
		auto output = average_output(outputs);

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

		args_["data"] = NDArray(Shape(symmetrics_, process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE), Context::cpu());
		args_["softmax_label"] = NDArray(Shape(symmetrics_), Context::cpu());
		net_.InferArgsMap(Context::cpu(), &args_, args_);

		exec_ = net_.SimpleBind(Context::cpu(), args_);
		LoadCheckpoint("policy.params", exec_);
	}

	boost::multi_array<mx_float, 4> PolicyNet::make_input(const Board& board)
	{
		boost::multi_array<mx_float, 4> ret { boost::extents[symmetrics_][process_.GetOutputDim()][BOARD_SIZE][BOARD_SIZE] };

		auto origin = process_.StateToTensor(board);
		for (size_t i = 0; i < symmetrics_; ++i)
		{
			if (i == 0) 	 ret[i] = origin;
			else if (i == 1) ret[i] = Symmetrics::Rot90(origin);
			else if (i == 2) ret[i] = Symmetrics::Rot180(origin);
			else if (i == 3) ret[i] = Symmetrics::Rot270(origin);
			else if (i == 4) ret[i] = Symmetrics::FlipUD(origin);
			else if (i == 5) ret[i] = Symmetrics::FlipLR(origin);
			else if (i == 6) ret[i] = Symmetrics::Diag1(origin);
			else if (i == 7) ret[i] = Symmetrics::Diag2(origin);
		}

		return ret;
	}

	boost::multi_array<mx_float, 2> PolicyNet::average_output(const Tensor& outputs)
	{
		boost::multi_array<mx_float, 2> ret { boost::extents[BOARD_SIZE][BOARD_SIZE] };
		std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

		for (size_t w = 0; w < symmetrics_; ++w)
		{
			auto tmp = [&]() {
				if (w == 0) 	 return outputs;
				else if (w == 1) return Symmetrics::DRot90(outputs);
				else if (w == 2) return Symmetrics::DRot180(outputs);
				else if (w == 3) return Symmetrics::DRot270(outputs);
				else if (w == 4) return Symmetrics::DFlipUD(outputs);
				else if (w == 5) return Symmetrics::DFlipLR(outputs);
				else if (w == 6) return Symmetrics::DDiag1(outputs);
				else if (w == 7) return Symmetrics::DDiag2(outputs);
			}();

			for (size_t i = 0; i < BOARD_SIZE; ++i)
				for (size_t j = 0; j < BOARD_SIZE; ++j)
					ret[i][j] += tmp[w][i][j];
		}

		for (size_t i = 0; i < BOARD_SIZE; ++i)
			for (size_t j = 0; j < BOARD_SIZE; ++j)
				ret[i][j] /= symmetrics_;

		return ret;
	}
}
