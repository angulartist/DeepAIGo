#include "Board.h"

#include <algorithm>
#include <stdio.h>

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
			throw std::runtime_error("Invalid move");

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

	void Board::ShowBoard() const
	{
		printf("   ");

		std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";

		for (int i = 0; i < BOARD_SIZE; ++i)
			printf("%c", coord[i]);

		printf("\n");

		for (int y = BOARD_SIZE - 1; y >= 0; --y)
		{
			printf("%02d ", (int)y + 1);

			for (size_t x = 0; x < BOARD_SIZE; ++x)
			{
				if (GetStoneColor(Point(x, y)) == StoneType::EMPTY)
					printf("·");
				else if (GetStoneColor(Point(x, y)) == StoneType::BLACK)
					printf("B");
				else
					printf("W");
			}

			printf("\n");
		}
	}

	void Board::ShowLiberties() const
	{
		printf("   ");

		std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";

		for (size_t i = 0; i < BOARD_SIZE; ++i)
			printf("%c ", coord[i]);

		printf("\n");

		for (int y = BOARD_SIZE - 1; y >= 0; --y)
		{
			printf("%02d", (int)y + 1);

			for (size_t x = 0; x < BOARD_SIZE; ++x)
			{
				printf("%02d", liberty_count_[POS(Point(x, y))]);
			}

			printf("\n");
		}
	}

	void Board::ShowGroups() const
	{
		printf("   ");

		std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";

		for (size_t i = 0; i < BOARD_SIZE; ++i)
			printf("%c ", coord[i]);

		printf("\n");

		for (int y = BOARD_SIZE - 1; y >= 0; --y)
		{
			printf("%02d", (int)y + 1);

			for (size_t x = 0; x < BOARD_SIZE; ++x)
			{
				printf("%02d", groups_[POS(Point(x, y))].size());
			}

			printf("\n");
		}
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
}