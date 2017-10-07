#include "HDF5Provider.h"

namespace mxcpp = mxnet::cpp;

HDF5Provider::HDF5Provider(const std::string& data, int batch_size, mxnet::cpp::Context dev)
	: device_(dev), batch_size_(batch_size)
{
	load_dataset(data);
}

HDF5Provider::~HDF5Provider()
{
	H5Dclose(states);
	H5Sclose(s_fspace);
	H5Dclose(actions);
	H5Sclose(a_fspace);
	H5Fclose(file);
}

void HDF5Provider::BeforeFirst()
{
	current_ = -1;
	real_samples_ = batch_size_ * (actions_dim[0] / batch_size_);
}

bool HDF5Provider::Next()
{
	return ++current_ < real_samples_ / batch_size_;
}

mxcpp::NDArray HDF5Provider::GetData()
{
	mx_float* data = new mx_float[states_dim[1] * states_dim[2] * states_dim[3] * batch_size_];
	hsize_t offset[4] = { current_ * batch_size_, 0, 0, 0 };
	auto shape = states_dim; shape[0] = batch_size_;

	H5Sselect_hyperslab(s_fspace, H5S_SELECT_SET, offset, NULL, shape, NULL);
	
	auto mspace = H5Screate_simple(4, shape, NULL);
	H5Dread(states, H5T_NATIVE_FLOAT, mspace, s_fspace, H5P_DEFAULT, data);

	H5Sclose(mspace);

	mxcpp::NDArray arr(mxcpp::Shape(shape[0], shape[1], shape[2], shape[3]), device_, false);
	arr.SyncCopyFromCPU(data,
		shape[0] * shape[1] * shape[2] * shape[3]);

	delete[] data;
	return arr;
}

mxcpp::NDArray HDF5Provider::GetLabel()
{
	mx_float* label = new mx_float[batch_size_];
	hsize_t offset[1] = { current_ * batch_size_ };
	hsize_t shape[1] = { batch_size_ };

	H5Sselect_hyperslab(a_fspace, H5S_SELECT_SET, offset, NULL, shape, NULL);

	auto mspace = H5Screate_simple(1, shape, NULL);
	H5Dread(actions, H5T_NATIVE_FLOAT, mspace, a_fspace, H5P_DEFAULT, label);

	H5Sclose(mspace);

	mxcpp::NDArray arr(mxcpp::Shape(batch_size_), device_, false);
	arr.SyncCopyFromCPU(label,
		batch_size_);

	delete[] label;
	return arr;
}

void HDF5Provider::load_dataset(const std::string& data)
{
	file = H5Fopen(data.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

	states = H5Dopen2(file, "states", H5P_DEFAULT);
	s_fspace = H5Dget_space(states);
	H5Sget_simple_extent_dims(s_fspace, states_dim, NULL);

	actions = H5Dopen2(file, "actions", H5P_DEFAULT);
	a_fspace = H5Dget_space(actions);
	H5Sget_simple_extent_dims(a_fspace, actions_dim, NULL);
}
