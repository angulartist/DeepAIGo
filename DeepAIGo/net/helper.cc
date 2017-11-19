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

#include "helper.h"

namespace mxnet
{
	namespace cpp
	{
		inline Symbol BatchNorm(const std::string& symbol_name,
			Symbol data,
			mx_float eps = 0.001,
			mx_float momentum = 0.9,
			bool fix_gamma = true,
			bool use_global_stats = false) {
			return Operator("BatchNorm")
					.SetParam("eps", eps)
					.SetParam("momentum", momentum)
					.SetParam("fix_gamma", fix_gamma)
					.SetParam("use_global_stats", use_global_stats)
					.SetInput("data", data)
					.CreateSymbol(symbol_name);
		}
	}
}

namespace DeepAIGo
{
	mxnet::cpp::Symbol ConvFactory(mxnet::cpp::Symbol data, int num_filter,
		mxnet::cpp::Shape kernel, mxnet::cpp::Shape stride, mxnet::cpp::Shape pad,
		const std::string & name) {
		mxnet::cpp::Symbol conv_w("convolution" + name + "_weight"), conv_b("convolution" + name + "_bias");

		mxnet::cpp::Symbol conv = mxnet::cpp::Convolution("conv_" + name, data,
			conv_w, conv_b, kernel,
			num_filter, stride, mxnet::cpp::Shape(1, 1), pad);
		std::string name_suffix = name;
		mxnet::cpp::Symbol bn = mxnet::cpp::BatchNorm("batchnorm" + name, conv);

		return Activation("acivation" + name, bn, "relu");
	}

	void LoadCheckpoint(const std::string& filename, mxnet::cpp::Executor* exe)
	{
		using namespace mxnet::cpp;

		std::map<std::string, NDArray> params = NDArray::LoadToMap(filename);
		for (auto iter : params) {
			std::string type = iter.first.substr(0, 4);
			std::string name = iter.first.substr(4);
			NDArray target;
			if (type == "arg:")
				target = exe->arg_dict()[name];
			else if (type == "aux:")
				target = exe->aux_dict()[name];
			else
				continue;
			iter.second.CopyTo(&target);
		}
	}

	void SaveCheckpoint(const std::string& filepath, mxnet::cpp::Symbol net, mxnet::cpp::Executor* exe)
	{
		using namespace mxnet::cpp;

		std::map<std::string, NDArray> params;
		for (auto iter : exe->arg_dict())
			if (iter.first.find("_init_") == std::string::npos
				&& iter.first.rfind("data") != iter.first.length() - 4
				&& iter.first.rfind("label") != iter.first.length() - 5)
				params.insert({ "arg:" + iter.first, iter.second });
		for (auto iter : exe->aux_dict())
			params.insert({ "aux:" + iter.first, iter.second });
		NDArray::Save(filepath, params);
	}
}
