#include "Converter.h"

#include <boost/filesystem.hpp>

#include <io/SgfParser.h>
#include <preprocess/Preprocess.h>

#include <iostream>

using namespace DeepAIGo;

Converter::Converter() :
	process_({ ProcessorType::STONE_COLOR, ProcessorType::ONES, ProcessorType::TURNS_SINCE, ProcessorType::LIBERTIES,
		ProcessorType::CAPTURE_SIZE, ProcessorType::SELF_ATARI_SIZE, ProcessorType::LIBERTIES_AFTER_MOVE, ProcessorType::SENSIBLENESS,
		ProcessorType::ZEROS })
{
	memset(s_offset, 0, sizeof(s_offset));
	memset(a_offset, 0, sizeof(a_offset));
}

void Converter::Convert(const std::string& folder, const std::string& filename)
{
	namespace fs = boost::filesystem;

	if (!fs::exists(folder))
		throw std::runtime_error("Invalid folder");

	file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

	hsize_t states_dim[4] = { 0, process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE };
	hsize_t states_mdim[4] = { H5S_UNLIMITED, process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE };
	hsize_t states_chunk[4] = { 64, process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE };

	hsize_t actions_dim[1] = { 0 };
	hsize_t actions_mdim[1] = { H5S_UNLIMITED };
	hsize_t actions_chunk[1] = { 64 };

	s_dspace = H5Screate_simple(4, states_dim, states_mdim);
	a_dspace = H5Screate_simple(1, actions_dim, actions_mdim);

	auto cparams = H5Pcreate(H5P_DATASET_CREATE);
	H5Pset_chunk(cparams, 4, states_chunk);
	states = H5Dcreate2(file, "/states", H5T_NATIVE_FLOAT, s_dspace, H5P_DEFAULT, cparams, H5P_DEFAULT);

	auto c2params = H5Pcreate(H5P_DATASET_CREATE);
	H5Pset_chunk(c2params, 1, actions_chunk);
	actions = H5Dcreate2(file, "/actions", H5T_NATIVE_FLOAT, a_dspace, H5P_DEFAULT, c2params, H5P_DEFAULT);
	
	unsigned long filenum = 0;

	fs::directory_iterator end;
	for (fs::directory_iterator it(folder); it != end; ++it)
	{
		std::cout << it->path().filename().generic_string();

		try
		{
			auto board = DeepAIGo::SgfParser::LoadFromFile(it->path().generic_string());

			std::vector<TrainingSet> ts;
			create_training_set(board, ts);

			auto state_ndim = states_dim; state_ndim[0] += ts.size();
			auto action_ndim = actions_dim; action_ndim[0] += ts.size();
	
			H5Dset_extent(states, state_ndim);
			H5Dset_extent(actions, action_ndim);
	
			s_fspace = H5Dget_space(states);
			a_fspace = H5Dget_space(actions);
	
			write_training_set(ts);
	
			++filenum;
			std::cout << std::endl;
		}
		catch (std::exception& e)
		{
			std::cerr << " <Exception> " << e.what() << std::endl;
			continue;
		}
	}

	H5Dclose(states);
	H5Dclose(actions);
	H5Sclose(s_fspace);
	H5Sclose(a_fspace);
	H5Sclose(s_dspace);
	H5Sclose(a_dspace);
	H5Fclose(file);

	std::cout << "[Result]" << std::endl 
			<< "Total files: " << filenum << std::endl
			<< "Total states: " << s_offset[0] << std::endl;
}

void Converter::create_training_set(const Board& board, std::vector<TrainingSet>& ts)
{
	Board tmp;

	for (auto& handicap : board.GetHandicap())
	{
		tmp.PlaceHandicap(handicap);
	}
	
	for (auto& act : board.GetHistory())
	{
		auto state = process_.StateToTensor(tmp);
		float action = (float)(act.X + act.Y * BOARD_SIZE);

		if (act != Pass)
			ts.push_back(std::make_tuple(state, action));
		else
			ts.push_back(std::make_tuple(state, BOARD_SIZE2));

		tmp.DoMove(act);
	}
}

void Converter::write_training_set(const std::vector<TrainingSet>& ts)
{
	hsize_t s_dim[4] = { ts.size(), process_.GetOutputDim(), BOARD_SIZE, BOARD_SIZE };
	hsize_t a_dim[1] = { ts.size() };

	boost::multi_array<float, 4> state{ boost::extents[ts.size()][process_.GetOutputDim()][BOARD_SIZE][BOARD_SIZE] };
	boost::multi_array<float, 1> action{ boost::extents[ts.size()] };

	for (size_t i = 0; i < ts.size(); ++i)
	{
		std::move(std::get<0>(ts[i]).begin(), std::get<0>(ts[i]).end(), state[i].begin());
		action[i] = std::get<1>(ts[i]);
	}

	H5Sselect_hyperslab(s_fspace, H5S_SELECT_SET, s_offset, NULL, s_dim, NULL);
	auto s_mspace = H5Screate_simple(4, s_dim, NULL);
	
	H5Dwrite(states, H5T_NATIVE_FLOAT, s_mspace, s_fspace, H5P_DEFAULT, state.data());

	H5Sselect_hyperslab(a_fspace, H5S_SELECT_SET, a_offset, NULL, a_dim, NULL);
	auto a_mspace = H5Screate_simple(1, a_dim, NULL);

	H5Dwrite(actions, H5T_NATIVE_FLOAT, a_mspace, a_fspace, H5P_DEFAULT, action.data());

	s_offset[0] += ts.size();
	a_offset[0] += ts.size();

	H5Sclose(s_mspace);
	H5Sclose(a_mspace);
}
