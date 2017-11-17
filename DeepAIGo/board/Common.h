#pragma once

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <set>

namespace DeepAIGo
{
	//! 바둑판의 줄 개수
	constexpr int BOARD_SIZE = 19;
	//! 바둑판 전체 칸 수
	constexpr int BOARD_SIZE2 = BOARD_SIZE * BOARD_SIZE;

	//! 덤
	constexpr float KOMI = 7.5f;

	//! 바둑돌 색상
	enum class StoneType
	{
		EMPTY,
		BLACK,
		WHITE
	};

	/** 해당 바둑돌의 상대 색상을 구합니다.
	 * @return 상대방 색상
	 * @param color 자신의 색상
	 **/
	constexpr StoneType GetOpponent(StoneType color)
	{
		switch (color)
		{
		case StoneType::BLACK: return StoneType::WHITE;
		case StoneType::WHITE: return StoneType::BLACK;
		default: return StoneType::EMPTY;
		}
	}

	//! 좌표를 표현하는 클래스입니다.
	struct Point
	{
		Point() : X(0), Y(0) { }
		Point(int x, int y) : X(x), Y(y) { }

		bool operator==(const Point& pt) const
		{
			return (X == pt.X && Y == pt.Y);
		}

		bool operator!=(const Point& pt) const
		{
			return (X != pt.X || Y != pt.Y);
		}

		bool operator<(const Point& pt) const
		{
			return (X + Y * 100) < (pt.X + pt.Y * 100);
		}

		bool operator>(const Point& pt) const
		{
			return (X + Y * 100) < (pt.X + pt.Y * 100);
		}

		int X, Y;
	};

	//! 한수 쉼
	const Point Pass = Point(-100, -100);
	//! 항복
	const Point Resign = Point(-200, -200);

	//! 돌의 무리
	using PointArr = std::vector<Point>;
	//! 돌의 집합
	using PointSet = std::set<Point>;

	/** 좌표를 1차원 index로 변환합니다.
	 * @return 1차원 index
	 * @param pt 좌표
	 **/
	constexpr int POS(const Point& pt)
	{
		return (pt.X + pt.Y * BOARD_SIZE);
	}

	/** 1차원 index를 좌표로 변환합니다.
	 * @return 좌표
	 * @param idx 1차원 index
	 **/
	inline Point IDX2PT(int idx)
	{
		return Point(idx % BOARD_SIZE, idx / BOARD_SIZE);
	}

	//! 좌표 사용자 정의 리터럴
	inline Point operator""_pt(const char* str, size_t len)
	{
		if (len > 3 || len < 2)
			throw std::runtime_error("Invalid coordinate");

		static std::string coord = "abcdefghjklmnopqrstuvwxyz";
		std::string tmp = str;
		boost::to_lower(tmp);
		
		return Point((int)coord.find_first_of(tmp[0]), atoi(tmp.substr(1).c_str()) - 1);
	}
}
