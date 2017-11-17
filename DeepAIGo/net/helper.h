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
