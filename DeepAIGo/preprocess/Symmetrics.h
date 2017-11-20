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

#pragma once

#include <boost/multi_array.hpp>

namespace DeepAIGo
{
	using Tensor = boost::multi_array<float, 3>;

    //! 데이터 변형 유틸리티
    class TensorUtil
    {
    public:
        /** 데이터를 반시계 방향으로 90도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rotl90(const Tensor& tensor);
        /** 데이터를 반시계 방향으로 180도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rotl180(const Tensor& tensor);
        /** 데이터를 반시계 방향으로 270도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rotl270(const Tensor& tensor);
        /** 데이터를 시계 방향으로 90도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rotr90(const Tensor& tensor);
        /** 데이터를 시계 방향으로 180도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rotr180(const Tensor& tensor);
        /** 데이터를 시계 방향으로 270도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rotr270(const Tensor& tensor);
        /** 데이터를 대각선을 기준으로 대칭이동 시킵니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Transpose(const Tensor& tensor);
        /** 데이터를 상하로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor FlipUD(const Tensor& tensor);
        /** 데이터를 좌우로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor FlipLR(const Tensor& tensor);
    };

    //! 데이터 변환 유틸리티
    class Symmetrics
    {
    public:
        /** 데이터를 90도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rot90(const Tensor& tensor) { return TensorUtil::Rotl90(tensor); }
         /** 데이터를 180도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rot180(const Tensor& tensor) { return TensorUtil::Rotl180(tensor); }
         /** 데이터를 270도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Rot270(const Tensor& tensor) { return TensorUtil::Rotl270(tensor); }
         /** 데이터를 상하로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor FlipUD(const Tensor& tensor) { return TensorUtil::FlipUD(tensor); }
         /** 데이터를 좌우로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor FlipLR(const Tensor& tensor) { return TensorUtil::FlipLR(tensor); }
         /** 데이터를 대각선1을 기준으로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Diag1(const Tensor& tensor) { return TensorUtil::Transpose(tensor); }
        /** 데이터를 대각선2을 기준으로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor Diag2(const Tensor& tensor) { return TensorUtil::Transpose(
                                                            TensorUtil::Rotl90(tensor)); }
        
        /** 데이터를 반대로 90도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DRot90(const Tensor& tensor) { return TensorUtil::Rotr90(tensor); }
         /** 데이터를 반대로 180도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DRot180(const Tensor& tensor) { return TensorUtil::Rotr180(tensor); }
         /** 데이터를 반대로 270도 회전합니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DRot270(const Tensor& tensor) { return TensorUtil::Rotr270(tensor); }
         /** 데이터를 반대로 상하로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DFlipUD(const Tensor& tensor) { return TensorUtil::FlipUD(tensor); }
         /** 데이터를 반대로 좌우로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DFlipLR(const Tensor& tensor) { return TensorUtil::FlipLR(tensor); }
         /** 데이터를 반대로 대각선1을 기준으로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DDiag1(const Tensor& tensor) { return TensorUtil::Transpose(tensor); }
        /** 데이터를 반대로 대각선2를 기준으로 뒤집습니다.
         * @return 변환된 데이터
         * @param tensor 입력 데이터
         **/
        static Tensor DDiag2(const Tensor& tensor) { return TensorUtil::Rotr90(
                                                            TensorUtil::Transpose(tensor)); }
    };
}