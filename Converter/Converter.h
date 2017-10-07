#pragma once

#include <string>
#include <hdf5.h>
#include <tuple>
#include <vector>

#include <preprocess/Preprocess.h>

using TrainingSet = std::tuple<DeepAIGo::Tensor, float>;

class Converter
{
public:
	Converter();

	void Convert(const std::string& folder, const std::string& filename);

private:
	void create_training_set(const DeepAIGo::Board& board, std::vector<TrainingSet>& ts);
	void write_training_set(const std::vector<TrainingSet>& ts);

private:
	hid_t file;
	hid_t s_dspace, a_dspace;
	hid_t s_fspace, a_fspace;
	hid_t states, actions;

	hsize_t s_offset[4];
	hsize_t a_offset[1];

	DeepAIGo::Preprocess process_;
};
