#include "ValueNet.h"

namespace DeepAIGo
{
    ValueNet::ValueNet()
        : process_({ ProcessorType::STONE_COLOR, ProcessorType::ONES, ProcessorType::TURNS_SINCE, ProcessorType::LIBERTIES,
			ProcessorType::CAPTURE_SIZE, ProcessorType::SELF_ATARI_SIZE, ProcessorType::LIBERTIES_AFTER_MOVE, ProcessorType::SENSIBLENESS,
            ProcessorType::ZEROS, ProcessorType::PLAYER_COLOR })
    {
        using namespace mxnet::cpp;

        Symbol data = Symbol::Variable("data");
        Symbol score = Symbol::Variable("score");

        std::vector<mxnet::cpp::Symbol> layers;
        layers.emplace_back(ConvFactory(data, 128, Shape(5, 5), Shape(1, 1), Shape(2, 2), "0"));

        for (int i = 1; i < 7; ++i)
        {
            layers.emplace_back(
                ConvFactory(layers.back(), 128, Shape(3, 3), Shape(1, 1), Shape(1, 1), std::to_string(i)));
        }

        layers.emplace_back(

        );
    }
}