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

	std::vector<ActionProb> PolicyNet::EvalState(const Board& board, int symmetric)
	{
		using namespace mxnet::cpp;

		auto input = make_input(board, symmetric);

		args_["data"].SyncCopyFromCPU(input.data(), input.num_elements());

		NDArray::WaitAll();

		exec_->Forward(false);
		
		Tensor outputs { boost::extents[1][1][BOARD_SIZE * BOARD_SIZE] };

		exec_->outputs[0].SyncCopyToCPU(outputs.data(), BOARD_SIZE2);
		NDArray::WaitAll();

		boost::array<Tensor::index, 3> dims = { { 1, BOARD_SIZE, BOARD_SIZE } };
		outputs.reshape(dims);
		auto output = convert_output(outputs, symmetric);

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

		args_["data"] = NDArray(Shape(1, process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE), Context::cpu());
		args_["softmax_label"] = NDArray(Shape(1), Context::cpu());
		net_.InferArgsMap(Context::cpu(), &args_, args_);

		exec_ = net_.SimpleBind(Context::cpu(), args_);
		LoadCheckpoint("policy.params", exec_);
	}

	boost::multi_array<mx_float, 4> PolicyNet::make_input(const Board& board, int symmetric)
	{
		boost::multi_array<mx_float, 4> ret { boost::extents[1][process_.GetOutputDim()][BOARD_SIZE][BOARD_SIZE] };

		auto origin = process_.StateToTensor(board);

		switch (symmetric)
		{
		case 0: ret[0] = origin; break;
		case 1: ret[0] = Symmetrics::Rot90(origin); break;
		case 2: ret[0] = Symmetrics::Rot180(origin); break;
		case 3: ret[0] = Symmetrics::Rot270(origin); break;
		case 4: ret[0] = Symmetrics::FlipUD(origin); break;
		case 5: ret[0] = Symmetrics::FlipLR(origin); break;
		case 6: ret[0] = Symmetrics::Diag1(origin); break;
		case 7: ret[0] = Symmetrics::Diag2(origin); break;
		default: throw std::runtime_error("Invalid symmetric");
		}

		return ret;
	}

	boost::multi_array<mx_float, 2> PolicyNet::convert_output(const Tensor& outputs, int symmetric)
	{
		boost::multi_array<mx_float, 2> ret { boost::extents[BOARD_SIZE][BOARD_SIZE] };		

		Tensor tmp { boost::extents[1][BOARD_SIZE][BOARD_SIZE] };
		switch (symmetric)
		{
		case 0: tmp = outputs; break;
		case 1: tmp = Symmetrics::DRot90(outputs); break;
		case 2: tmp = Symmetrics::DRot180(outputs); break;
		case 3: tmp = Symmetrics::DRot270(outputs); break;
		case 4: tmp = Symmetrics::DFlipUD(outputs); break;
		case 5: tmp = Symmetrics::DFlipLR(outputs); break;
		case 6: tmp = Symmetrics::DDiag1(outputs); break;
		case 7: tmp = Symmetrics::DDiag2(outputs); break;
		default: throw std::runtime_error("Invalid symmetric");
		}

		memcpy(ret.data(), tmp.data(), ret.num_elements() * sizeof(mx_float));

		return ret;
	}
}
