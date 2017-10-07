#pragma once

#include <mxnet-cpp/MxNetCpp.h>
#include <hdf5.h>

class HDF5Provider
{
public:
	HDF5Provider(const std::string& data, int batch_size, 
		mxnet::cpp::Context dev = mxnet::cpp::Context::cpu());
	~HDF5Provider();

	virtual void BeforeFirst(void);
	virtual bool Next(void);
	virtual mxnet::cpp::NDArray GetData(void);
	virtual mxnet::cpp::NDArray GetLabel(void);

private:
	void load_dataset(const std::string& data);

private:
	hid_t file;
	hid_t states, actions;
	hid_t s_fspace, a_fspace;
	hsize_t states_dim[4];
	hsize_t actions_dim[1];

	int batch_size_;
	int real_samples_;
	int current_;
	
	mxnet::cpp::Context device_;
};
