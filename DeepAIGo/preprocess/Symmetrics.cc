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

#include "Symmetrics.h"
#include "../board/Common.h"

namespace DeepAIGo
{
    Tensor TensorUtil::Rotl90(const Tensor& tensor)
    {
        Tensor ret { boost::extents[tensor.shape()[0]][BOARD_SIZE][BOARD_SIZE]};

        for (size_t w = 0; w < tensor.shape()[0]; ++w)
        {
            for (size_t i = 0; i < BOARD_SIZE; ++i)
            {
                for (size_t j = 0; j < BOARD_SIZE; ++j)
                {
                    ret[w][j][i] = tensor[w][i][BOARD_SIZE - j - 1];
                }
            }
        }

        return ret;
    }

    Tensor TensorUtil::Rotl180(const Tensor& tensor)
    {
        return Rotl90(Rotl90(tensor));
    }

    Tensor TensorUtil::Rotl270(const Tensor& tensor)
    {
        return Rotl90(Rotl180(tensor));
    }

    Tensor TensorUtil::Rotr90(const Tensor& tensor)
    {
        Tensor ret { boost::extents[tensor.shape()[0]][BOARD_SIZE][BOARD_SIZE]};

        for (size_t w = 0; w < tensor.shape()[0]; ++w)
        {
            for (size_t i = 0; i < BOARD_SIZE; ++i)
            {
                for (size_t j = 0; j < BOARD_SIZE; ++j)
                {
                    ret[w][j][i] = tensor[w][BOARD_SIZE - i - 1][j];
                }
            }
        }

        return ret;
    }

    Tensor TensorUtil::Rotr180(const Tensor& tensor)
    {
        return Rotr90(Rotr90(tensor));
    }

    Tensor TensorUtil::Rotr270(const Tensor& tensor)
    {
        return Rotr90(Rotr180(tensor));
    }

    Tensor TensorUtil::Transpose(const Tensor& tensor)
    {
        Tensor ret { boost::extents[tensor.shape()[0]][BOARD_SIZE][BOARD_SIZE]};

        for (size_t w = 0; w < tensor.shape()[0]; ++w)
        {
            for (size_t i = 0; i < BOARD_SIZE; ++i)
            {
                for (size_t j = 0; j < BOARD_SIZE; ++j)
                {
                    ret[w][j][i] = tensor[w][i][j];
                }
            }
        }

        return ret;
    }

    Tensor TensorUtil::FlipUD(const Tensor& tensor)
    {
        Tensor ret { boost::extents[tensor.shape()[0]][BOARD_SIZE][BOARD_SIZE] };

        for (size_t w = 0; w < tensor.shape()[0]; ++w)
        {
            for (size_t i = 0; i < BOARD_SIZE; ++i)
            {
                for (size_t j = 0; j < BOARD_SIZE; ++j)
                {
                    ret[w][i][j] = tensor[w][BOARD_SIZE - i - 1][j];
                }
            }
        }

        return ret;
    }

    Tensor TensorUtil::FlipLR(const Tensor& tensor)
    {
        Tensor ret { boost::extents[tensor.shape()[0]][BOARD_SIZE][BOARD_SIZE] };

        for (size_t w = 0; w < tensor.shape()[0]; ++w)
        {
            for (size_t i = 0; i < BOARD_SIZE; ++i)
            {
                for (size_t j = 0; j < BOARD_SIZE; ++j)
                {
                    ret[w][i][j] = tensor[w][i][BOARD_SIZE - j - 1];
                }
            }
        }

        return ret;
    }
}