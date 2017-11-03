#pragma once

#include <boost/multi_array.hpp>

namespace DeepAIGo
{
	using Tensor = boost::multi_array<float, 3>;

    class TensorUtil
    {
    public:
        static Tensor Rotl90(const Tensor& tensor);
        static Tensor Rotl180(const Tensor& tensor);
        static Tensor Rotl270(const Tensor& tensor);
        static Tensor Rotr90(const Tensor& tensor);
        static Tensor Rotr180(const Tensor& tensor);
        static Tensor Rotr270(const Tensor& tensor);
        static Tensor Transpose(const Tensor& tensor);
        static Tensor FlipUD(const Tensor& tensor);
        static Tensor FlipLR(const Tensor& tensor);
    };

    class Symmetrics
    {
    public:
        static Tensor Rot90(const Tensor& tensor) { return TensorUtil::Rotl90(tensor); }
        static Tensor Rot180(const Tensor& tensor) { return TensorUtil::Rotl180(tensor); }
        static Tensor Rot270(const Tensor& tensor) { return TensorUtil::Rotl270(tensor); }
        static Tensor FlipUD(const Tensor& tensor) { return TensorUtil::FlipUD(tensor); }
        static Tensor FlipLR(const Tensor& tensor) { return TensorUtil::FlipLR(tensor); }
        static Tensor Diag1(const Tensor& tensor) { return TensorUtil::Transpose(tensor); }
        static Tensor Diag2(const Tensor& tensor) { return TensorUtil::Transpose(
                                                            TensorUtil::Rotl90(tensor)); }
    };
}