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