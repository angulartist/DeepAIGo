#pragma once

#include "HDF5Provider.h"

#include <net/PolicyNet.h>
#include <memory>

class PolicyTrainer : public DeepAIGo::PolicyNet
{
public:
	PolicyTrainer(const std::string& train, const std::string& test, int batch_size, int epoch);
	virtual ~PolicyTrainer();

	void Train(float lr);

	virtual void InitNetwork();

private:
	std::shared_ptr<HDF5Provider> train_;
	std::shared_ptr<HDF5Provider> test_;

	int batch_size_;
	int epoch_;
};
