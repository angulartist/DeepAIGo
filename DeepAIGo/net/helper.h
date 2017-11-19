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

#include <mxnet-cpp/MxNetCpp.h>
#include <mxnet-cpp/op.h>

namespace DeepAIGo
{
	/** Convolution Layer를 생성합니다.
	 * @return Convolution Layer
	 * @param data 이전 레이어
	 * @param num_filter 필터 개수
	 * @param kernel 커널 크기
	 * @param stride stride 크기
	 * @param pad padding 크기
	 * @param name 레이어 이름
	 **/
	mxnet::cpp::Symbol ConvFactory(mxnet::cpp::Symbol data, int num_filter,
		mxnet::cpp::Shape kernel, mxnet::cpp::Shape stride, mxnet::cpp::Shape pad,
		const std::string & name);

	/** 신경망 정보를 저장합니다.
	 * @param filename 저장할 파일 경로
	 * @param exe Executor
	 **/
	void LoadCheckpoint(const std::string& filename, mxnet::cpp::Executor* exe);
	/** 신경망 정보를 불러옵니다.
	 * @param filename 신경망 정보 파일 경로
	 * @param net 신경망
	 * @param exe Executor
	 **/
	void SaveCheckpoint(const std::string& filename, mxnet::cpp::Symbol net, mxnet::cpp::Executor* exe);
}
