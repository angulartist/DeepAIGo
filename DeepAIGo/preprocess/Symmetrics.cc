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