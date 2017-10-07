#pragma once

#include "../board/Board.h"

namespace DeepAIGo
{
	class SgfParser
	{
	public:
		static Board Parse(const std::string& sgf);
		static Board LoadFromFile(const std::string& filename);
	};
}
