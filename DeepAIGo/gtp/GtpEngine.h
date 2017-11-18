#pragma once

#include "../board/Board.h"

#include <boost/noncopyable.hpp>
#include <vector>
#include <iostream>

namespace DeepAIGo
{
	//! GTP 명령 타입
	enum class GtpCmdType
	{
		QUIT,
		PROTOCOL_VERSION,
		NAME,
		VERSION,
		CLEAR_BOARD,
		SHOW_BOARD,
		PLAY,
		GENMOVE,
		UNKNOWN
	};

	//! GTP 명령
	struct GtpCmd
	{
		GtpCmdType command;
		std::vector<std::string> args;

		GtpCmd() : command(GtpCmdType::UNKNOWN) { }
		GtpCmd(GtpCmdType cmd) : command(cmd) { }
	};

	//! Go Text Protocol 엔진
	class GtpEngine : public boost::noncopyable
	{
	public:
		GtpEngine(const std::string& name, const std::string& version);
		virtual ~GtpEngine() = default;

		//! 엔진이 작동중인지 확인합니다.
		bool IsRunning() const;
		/** 엔진을 작동합니다.
		 * @param stream 입력 스트림
		 * @param outstream 출력 스트림
		 **/
		void Run(std::istream& stream, std::ostream& outstream);

		/** 명령어를 처리합니다.
		 * @return 처리 결과
		 * @param command GTP 명령
		 **/
		virtual std::string Process(const GtpCmd& command) = 0;

	protected:
		/** 색상을 파싱합니다.
		 * @return 색상
		 * @param arg 색상 문자열
		 **/
		StoneType parse_color(const std::string& arg);
		/** 좌표를 파싱합니다.
		 * @return 좌표
		 * @param arg 좌표 문자열
		 **/
		Point parse_coord(const std::string& arg);
		/** 좌표를 문자열로 변환합니다.
		 * @return 좌표 문자열
		 * @param pt 좌표
		 **/
		std::string coord_to_str(const Point& pt);
		/** 명령어를 파싱합니다.
		 * @return 명령어
		 * @param tokens 명령어 토큰
		 **/
		GtpCmd parse_command(const std::vector<std::string>& tokens);

	private:
		//! AI의 이름
		std::string name_;
		//! AI 버전
		std::string version_;
		//! 엔진 작동 여부
		bool is_run_;
		//! 실행된 명령어 개수
		int cmd_idx_;
	};
}
