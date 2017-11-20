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

#include "Board.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace DeepAIGo
{
	Board::Board()
	{
		board_.resize(BOARD_SIZE2);
		liberty_count_.resize(BOARD_SIZE2);
		liberties_.resize(BOARD_SIZE2);
		groups_.resize(BOARD_SIZE2);

		Clear();
	}

	void Board::Clear()
	{
		std::fill(board_.begin(), board_.end(), StoneType::EMPTY);
		
		for (int x = 0; x < BOARD_SIZE; ++x)
		{
			for (int y = 0; y < BOARD_SIZE; ++y)
			{
				auto lib = GetNeighbor(Point(x, y));
				liberties_[POS(Point(x, y))].clear();
				liberties_[POS(Point(x, y))].insert(lib.begin(), lib.end());
				groups_[POS(Point(x, y))].clear();
			}
		}

		std::fill(liberty_count_.begin(), liberty_count_.end(), -1);
		
		current_player_ = StoneType::BLACK;
		ko_ = Pass;
		is_ended_ = false;
		history_.clear();

		black_captured_ = 0;
		white_captured_ = 0;
	}

	Board Board::Copy() const
	{
		Board ret;
		
		std::copy(board_.begin(), board_.end(), ret.board_.begin());
		std::copy(liberties_.begin(), liberties_.end(), ret.liberties_.begin());
		std::copy(groups_.begin(), groups_.end(), ret.groups_.begin());
		std::copy(liberty_count_.begin(), liberty_count_.end(), ret.liberty_count_.begin());

		ret.current_player_ = current_player_;
		ret.ko_ = ko_;
		ret.is_ended_ = is_ended_;
		ret.history_.assign(history_.begin(), history_.end());

		ret.black_captured_ = black_captured_;
		ret.white_captured_ = white_captured_;

		return ret;
	}

	bool Board::IsValidMove(const Point& pt) const
	{
		if (pt == Pass || pt == Resign)
			return true;

		return IsOnBoard(pt) &&
			board_[POS(pt)] == StoneType::EMPTY &&
			ko_ != pt &&
			!IsSuicide(pt);
	}

	bool Board::IsBoundary(const Point& pt) const
	{
		return (pt.X == -1 || pt.Y == -1) || (pt.X == BOARD_SIZE + 1 || pt.Y == BOARD_SIZE + 1);
	}

	bool Board::IsOnBoard(const Point& pt) const
	{
		return (pt.X >= 0 && pt.Y >= 0) && (pt.X < BOARD_SIZE && pt.Y < BOARD_SIZE);
	}

	bool Board::IsSuicide(const Point& pt) const
	{
		if (liberties_[POS(pt)].size() == 0)
		{
			for (auto& n : GetNeighbor(pt))
			{
				auto lib = liberties_[POS(n)];

				auto it = std::find(lib.begin(), lib.end(), pt);
				if (it != lib.end())
					lib.erase(it);

				if (board_[POS(n)] == current_player_ && lib.size() > 0)
					return false;
				else if (board_[POS(n)] == GetOpponent(current_player_) && lib.size() == 0)
					return false;
			}

			return true;
		}

		return false;
	}

	bool Board::IsEyeShape(const Point& pt, StoneType owner) const
	{
		if (board_[POS(pt)] != StoneType::EMPTY)
			return false;

		for (auto& n : GetNeighbor(pt))
			if (board_[POS(n)] != owner)
				return false;

		return true;
	}

	bool Board::IsTrueEye(const Point& pt, StoneType owner) const
	{
		int num_boundary = 0;
		int num_opponent = 0;

		for (auto& d : GetDiagonal(pt))
		{
			if (IsBoundary(d))
				++num_boundary;
			if (board_[POS(d)] == GetOpponent(owner))
				++num_opponent;
		}

		return IsEyeShape(pt, owner) &&
			!((num_boundary > 0 && num_opponent > 0) || (num_boundary == 0 && num_opponent > 1));
	}

	StoneType Board::GetEyeColor(const Point& pt) const
	{
		if (IsTrueEye(pt, StoneType::BLACK)) return StoneType::BLACK;
		else if (IsTrueEye(pt, StoneType::WHITE)) return StoneType::WHITE;
		else return StoneType::EMPTY;
	}

	bool Board::IsEnded() const
	{
		return is_ended_;
	}

	StoneType Board::GetStoneColor(const Point& pt) const
	{
		return board_[POS(pt)];
	}

	StoneType Board::GetCurrentPlayer() const
	{
		return current_player_;
	}

	StoneType Board::GetOpponentPlayer() const
	{
		return GetOpponent(current_player_);
	}

	int Board::GetBlackCaptured() const
	{
		return black_captured_;
	}

	int Board::GetWhiteCaptured() const
	{
		return white_captured_;
	}

	const PointArr& Board::GetHistory() const
	{
		return history_;
	}

	PointArr Board::GetNeighbor(const Point& pt) const
	{
		PointArr ret;

		if (IsOnBoard(Point(pt.X + 1, pt.Y))) ret.emplace_back(pt.X + 1, pt.Y);
		if (IsOnBoard(Point(pt.X - 1, pt.Y))) ret.emplace_back(pt.X - 1, pt.Y);
		if (IsOnBoard(Point(pt.X, pt.Y + 1))) ret.emplace_back(pt.X, pt.Y + 1);
		if (IsOnBoard(Point(pt.X, pt.Y - 1))) ret.emplace_back(pt.X, pt.Y - 1);

		return ret;
	}

	PointArr Board::GetDiagonal(const Point& pt) const
	{
		PointArr ret;

		if (IsOnBoard(Point(pt.X + 1, pt.Y + 1))) ret.emplace_back(pt.X + 1, pt.Y + 1);
		if (IsOnBoard(Point(pt.X - 1, pt.Y + 1))) ret.emplace_back(pt.X - 1, pt.Y + 1);
		if (IsOnBoard(Point(pt.X + 1, pt.Y - 1))) ret.emplace_back(pt.X + 1, pt.Y - 1);
		if (IsOnBoard(Point(pt.X - 1, pt.Y - 1))) ret.emplace_back(pt.X - 1, pt.Y - 1);

		return ret;
	}

	const PointSet& Board::GetLiberties(const Point& pt) const
	{
		return liberties_[POS(pt)];
	}

	const PointSet& Board::GetGroup(const Point& pt) const
	{
		return groups_[POS(pt)];
	}

	const PointArr& Board::GetHandicap() const
	{
		return handicap_;
	}

	std::vector<PointSet> DeepAIGo::Board::GetNeighborGroups(const Point& pt) const
	{
		std::vector<PointSet> ret;

		for (auto& n : GetNeighbor(pt))
		{
			if (groups_[POS(n)].size() == 0) continue;

			auto it = std::find_if(ret.begin(), ret.end(), [&](const PointSet& v) {
				return *v.begin() == *groups_[POS(n)].begin();
			});

			if (it == ret.end())
				ret.push_back(groups_[POS(n)]);
		}

		return ret;
	}

	int Board::GetLibertyCount(const Point& pt) const
	{
		return liberty_count_[POS(pt)];
	}

	PointArr Board::GetLegalMoves() const
	{
		PointArr ret;

		for (int x = 0; x < BOARD_SIZE; ++x)
			for (int y = 0; y < BOARD_SIZE; ++y)
				if (IsValidMove(Point(x, y)))
					ret.emplace_back(x, y);

		return ret;
	}

	void Board::PlaceHandicap(const Point& pt)
	{
		handicap_.push_back(pt);

		DoMove(pt, StoneType::BLACK, false);
	}

	void Board::DoMove(const Point& pt, StoneType color, bool history)
	{
		if (!IsValidMove(pt))
		{ 
			std::cout << history_.size() << " (" << pt.X << ", " << pt.Y <<")" << std::endl;
			ShowBoard();
			ShowLibertyCount();

			std::cout << IsOnBoard(pt) << std::endl << IsSuicide(pt) << std::endl << (int)(ko_ == pt) << std::endl << (int)(board_[POS(pt)] == StoneType::EMPTY) << std::endl;

			throw std::runtime_error("Invalid move");
		}

		if (is_ended_)
			throw std::runtime_error("Already ended");

		current_player_ = color;
		ko_ = Pass;

		if (pt == Pass)
		{
			if (history_.size() > 0 && history_.back() == Pass)
				is_ended_ = true;
		}
		else if (pt == Resign)
		{
			is_ended_ = true;
		}
		else
		{
			board_[POS(pt)] = color;
			update_neighbor(pt);

			for (auto& n : GetNeighbor(pt))
			{
				if (board_[POS(n)] == GetOpponent(current_player_) && liberties_[POS(n)].size() == 0)
				{
					int captured = (int)groups_[POS(n)].size();
					remove_group(groups_[POS(n)]);

					if (current_player_ == StoneType::BLACK)
						white_captured_ += captured;
					else
						black_captured_ += captured;

					if (captured == 1 && liberties_[POS(pt)].size() == 1 && groups_[POS(pt)].size() == 1)
						ko_ = n;
				}
			}
		}

		current_player_ = GetOpponent(color);

		if (history)
			history_.push_back(pt);
	}

	void Board::DoMove(const Point& pt)
	{
		DoMove(pt, current_player_);
	}

	float Board::GetTrompTaylorScore() const
	{
		std::vector<int> territory;
		territory.resize(BOARD_SIZE2);

		for (size_t i = 0; i < BOARD_SIZE2; ++i)
			territory[i] = (int)board_[i];

		while (territory_floodfill(territory));

		float black_score = white_captured_;
		float white_score = black_captured_;

		for (size_t i = 0; i < BOARD_SIZE2; ++i)
		{
			if (territory[i] == 1) ++black_score;
			else if (territory[i] == 2) ++white_score;
		}

		return black_score - white_score;
	}

	void Board::ShowBoard() const
	{
		std::cout << "  " << this->ToString();
	}

	void Board::ShowTerritory() const
	{
		std::vector<int> territory;
		territory.resize(BOARD_SIZE2);

		for (size_t i = 0; i < BOARD_SIZE2; ++i)
			territory[i] = (int)board_[i];

		while (territory_floodfill(territory));

		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; ++i)
			std::cout  << (char)('A' + i);

		std::cout  << std::endl;

		for (int y = BOARD_SIZE - 1; y >= 0; --y)
		{
			std::cout  << std::setw(2) << std::setfill('0') << y+1 << " ";

			for (int x = 0; x < BOARD_SIZE; ++x)
			{
				if (GetStoneColor(Point(x, y)) == StoneType::EMPTY)
					if (territory[POS(Point(x, y))] == 1)
						std::cout  << "B";
					else if (territory[POS(Point(x, y))] == 2)
						std::cout  << "W";
					else
						std::cout << "·";
				else if (GetStoneColor(Point(x, y)) == StoneType::BLACK)
				std::cout  << "B";
				else
				std::cout  << "W";
			}

			std::cout  << std::endl;
		}
	}

	void Board::ShowLibertyCount() const
	{
		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; ++i)
			std::cout  << (char)('A' + i);

		std::cout  << std::endl;

		for (int y = BOARD_SIZE - 1; y >= 0; --y)
		{
			std::cout  << std::setw(2) << std::setfill('0') << y+1 << " ";

			for (int x = 0; x < BOARD_SIZE; ++x)
			{
				printf("%02d", liberty_count_[POS(Point(x, y))]);
			}

			std::cout  << std::endl;
		}
	}

	std::string Board::ToString() const
	{
		std::stringstream ss;

		for (int i = 0; i < BOARD_SIZE; ++i)
			ss << (char)('A' + i);

		ss << std::endl;

		for (int y = BOARD_SIZE - 1; y >= 0; --y)
		{
			ss << std::setw(2) << std::setfill('0') << y+1 << " ";

			for (int x = 0; x < BOARD_SIZE; ++x)
			{
				if (GetStoneColor(Point(x, y)) == StoneType::EMPTY)
					ss << "·";
				else if (GetStoneColor(Point(x, y)) == StoneType::BLACK)
					ss << "B";
				else
					ss << "W";
			}

			ss << std::endl;
		}

		return ss.str();
	}

	void Board::update_neighbor(const Point& pt)
	{
		PointSet new_group;
		PointSet new_liberties;
		new_group.insert(pt);
		new_liberties.insert(liberties_[POS(pt)].begin(), liberties_[POS(pt)].end());

		for (auto& n : GetNeighbor(pt))
		{
			auto& liberties = liberties_[POS(n)];

			auto it = std::find(liberties.begin(), liberties.end(), pt);
			if (it != liberties.end())
				liberties.erase(it);

			if (board_[POS(n)] == board_[POS(pt)])
			{
				new_group.insert(groups_[POS(n)].begin(), groups_[POS(n)].end());
				new_liberties.insert(liberties_[POS(n)].begin(), liberties_[POS(n)].end());
			}
			else if (board_[POS(n)] == GetOpponent(current_player_))
			{
				for (auto& g : groups_[POS(n)])
				{
					liberties_[POS(g)] = liberties;
					liberty_count_[POS(g)] = liberties.size();
				}
			}
		}

		for (auto& g : new_group)
		{
			groups_[POS(g)] = new_group;
			liberties_[POS(g)] = new_liberties;
			liberty_count_[POS(g)] = new_liberties.size();
		}
	}

	void Board::remove_group(PointSet group)
	{
		for (auto& g : group)
			board_[POS(g)] = StoneType::EMPTY;
		
		for (auto& g : group)
		{
			groups_[POS(g)].clear();
			liberties_[POS(g)].clear();
			liberty_count_[POS(g)] = -1;

			for (auto& n : GetNeighbor(g))
			{
				if (board_[POS(n)] == StoneType::EMPTY)
				{
					liberties_[POS(n)].insert(g);
					liberties_[POS(g)].insert(n);
				}
				else
				{
					for (auto& p : groups_[POS(n)])
					{
						liberties_[POS(p)].insert(g);
						liberty_count_[POS(p)] = liberties_[POS(p)].size();
					}
				}
			}
		}
	}

	bool Board::territory_floodfill(std::vector<int>& territory) const
	{
		bool ret = false;

		for (size_t i = 0; i < BOARD_SIZE2; ++i)
		{
			if (board_[i] == StoneType::EMPTY)
			{
				if (territory[i] == 3) continue; // if DAME

				int neighbors[4] = {0};

				for (auto& n : GetNeighbor(IDX2PT(i)))
					++neighbors[territory[POS(n)]];

				if ((neighbors[1] && neighbors[2]) || neighbors[3])
				{
					territory[i] = 3;

					for (auto& n : GetNeighbor(IDX2PT(i)))
						if (territory[POS(n)] == 0)
							territory[POS(n)] = 3;

					ret = true;
				}
				else if (territory[i] == 0 && neighbors[1])
				{
					territory[i] = 1;

					for (auto& n : GetNeighbor(IDX2PT(i)))
						if (territory[POS(n)] == 0)
							territory[POS(n)] = 1;

					ret = true;
				}
				else if (territory[i] == 0 && neighbors[2])
				{
					territory[i] = 2;

					for (auto& n : GetNeighbor(IDX2PT(i)))
						if (territory[POS(n)] == 0)
							territory[POS(n)] = 2;

					ret = true;
				}
			}
		}

		return ret;
	}
}