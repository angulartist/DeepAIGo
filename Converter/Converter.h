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
