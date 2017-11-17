#pragma once

#include "../board/Board.h"

namespace DeepAIGo
{
	//! SGF 파서
	class SgfParser
	{
	public:
		/** SGF를 파싱합니다.
		 * @return 파싱된 바둑판
		 * @param sgf SGF 내용
		 **/
		static Board Parse(const std::string& sgf);
		/** SGF 파일을 파싱합니다.
		 * @return 파싱된 바둑판
		 * @param filename sgf 파일 경로
		 **/
		static Board LoadFromFile(const std::string& filename);
	};
}
