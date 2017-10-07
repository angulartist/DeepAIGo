#include "PolicyTrainer.h"

#include <chrono>
#include <iostream>

using namespace DeepAIGo;
namespace mxcpp = mxnet::cpp;

PolicyTrainer::PolicyTrainer(const std::string& train, const std::string& test, int batch_size, int epoch)
	: epoch_(epoch), batch_size_(batch_size), PolicyNet()
{
	train_ = std::make_shared<HDF5Provider>(train, batch_size);
	test_ = std::make_shared<HDF5Provider>(test, batch_size);

	InitNetwork();
}

PolicyTrainer::~PolicyTrainer()
{
}

void PolicyTrainer::Train()
{
	mxcpp::Optimizer* opt = mxcpp::OptimizerRegistry::Find("ccsgd");
	opt->SetParam("lr", 0.003)
		->SetParam("wd", 0.0001)
		->SetParam("momentum", 0.9);

	for (int e = 0; e < epoch_; ++e)
	{
		train_->BeforeFirst();

		auto begin = std::chrono::system_clock::now();

		while (train_->Next())
		{
			train_->GetData().CopyTo(&exec_->arg_dict()["data"]);
			train_->GetLabel().CopyTo(&exec_->arg_dict()["softmax_label"]);
			mxcpp::NDArray::WaitAll();

			exec_->Forward(true);
			exec_->Backward();

			for (size_t i = 0; i < exec_->arg_arrays.size(); ++i)
				opt->Update(i, exec_->arg_arrays[i], exec_->grad_arrays[i]);

			mxcpp::NDArray::WaitAll();
		}

		auto end = std::chrono::system_clock::now();

		std::cout << "Epoch[" << e << "] Time Cost: " <<
			std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds" << std::endl;
	}

	SaveCheckpoint("test.params", net_, exec_);
}

void PolicyTrainer::InitNetwork()
{
	args_["data"] = mxcpp::NDArray(mxcpp::Shape(batch_size_, process_.GetOutputDim(), 19, 19), mxcpp::Context::cpu());
	args_["softmax_label"] = mxcpp::NDArray(mxcpp::Shape(batch_size_), mxcpp::Context::cpu());
	net_.InferArgsMap(mxcpp::Context::cpu(), &args_, args_);

	exec_ = net_.SimpleBind(mxcpp::Context::cpu(), args_);
}
