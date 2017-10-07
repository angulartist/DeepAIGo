#pragma once

#include <mxnet-cpp/MxNetCpp.h>
#include <mxnet-cpp/op.h>

namespace DeepAIGo
{
	mxnet::cpp::Symbol ConvFactory(mxnet::cpp::Symbol data, int num_filter,
		mxnet::cpp::Shape kernel, mxnet::cpp::Shape stride, mxnet::cpp::Shape pad,
		const std::string & name);

	void LoadCheckpoint(const std::string& filename, mxnet::cpp::Executor* exe);
	void SaveCheckpoint(const std::string& filename, mxnet::cpp::Symbol net, mxnet::cpp::Executor* exe);
}
