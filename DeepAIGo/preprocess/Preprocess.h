#pragma once

#include "../board/Board.h"

#include <algorithm>
#include <functional>
#include <vector>

#include "Symmetrics.h"

namespace DeepAIGo
{
	//! 전처리기 타입
	enum class ProcessorType
	{
		STONE_COLOR,			//! 바둑돌 색상
		ONES,					//! 1로 채워진 상수
		TURNS_SINCE,			//! 몇 수 전에 놓였는지
		LIBERTIES,				//! 활로 개수 (0~8)
		CAPTURE_SIZE,			//! 두면 따낼 수 있는 개수 (0~8)
		SELF_ATARI_SIZE,		//! 상대방이 뒀을 때 내가 잃을 돌의 개수 (0~8)
		LIBERTIES_AFTER_MOVE,	//! 두고난 후 활로의 개수 (0~8)
		SENSIBLENESS,			//! 바둑 규칙에 유효한지
		ZEROS,					//! 0으로 채워진 상수
		PLAYER_COLOR			//! 플레이어의 색상
	};

	//! 전처리
	struct Processor
	{
		//! 출력 데이터의 깊이
		size_t output_dim_;
		//! 전처리 구현부
		std::function<Tensor(const Board&)> impl_;

		Processor() { }
		/**
		 * @param output_dim 출력 데이터의 깊이
		 * @param impl 전처리 구현부
		 **/
		Processor(size_t output_dim, std::function<Tensor(const Board&)> impl) : impl_(impl), output_dim_(output_dim) { }
	};

	//! 전처리기
	class Preprocess
	{
	public:
		/**
		 * @param process 전처리 목록
		 **/
		Preprocess(const std::vector<ProcessorType>& process);

		//! 총 데이터의 깊이를 구합니다.
		size_t GetOutputDim() const;
		/** 바둑판 상태를 데이터로 변환합니다.
		 * @return 변환된 데이터
		 * @param board 바둑판 상태
		 **/
		Tensor StateToTensor(const Board& board);

	private:
		//! 전처리 목록
		std::vector<Processor> process_;
		//! 총 데이터의 깊이
		size_t output_dim_;
	};
}
