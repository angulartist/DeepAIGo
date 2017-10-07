#pragma once

#include "Common.h"

namespace DeepAIGo
{
	class Board
	{
	public:
		Board();
		virtual ~Board() = default;

		bool IsValidMove(const Point& pt) const;
		bool IsBoundary(const Point& pt) const;
		bool IsOnBoard(const Point& pt) const;
		bool IsSuicide(const Point& pt) const;

		bool IsEyeShape(const Point& pt, StoneType owner) const;
		bool IsTrueEye(const Point& pt, StoneType owner) const;

		bool IsEnded() const;
		StoneType GetStoneColor(const Point& pt) const;
		StoneType GetCurrentPlayer() const;
		StoneType GetOpponentPlayer() const;

		int GetBlackCaptured() const;
		int GetWhiteCaptured() const;
		const PointArr& GetHistory() const;
		const PointArr& GetHandicap() const;

		PointArr GetNeighbor(const Point& pt) const;
		PointArr GetDiagonal(const Point& pt) const;
		
		const PointSet& GetLiberties(const Point& pt) const;
		const PointSet& GetGroup(const Point& pt) const;
		std::vector<PointSet> GetNeighborGroups(const Point& pt) const;
		int GetLibertyCount(const Point& pt) const;
		PointArr GetLegalMoves() const;

		void PlaceHandicap(const Point& pt);
		void DoMove(const Point& pt, StoneType color, bool history = true);
		void DoMove(const Point& pt);

		void ShowBoard() const;
		void ShowGroups() const;
		void ShowLiberties() const;

	private:
		void update_neighbor(const Point& pt);
		void remove_group(PointSet group);

	private:
		StoneType current_player_;
		bool is_ended_;
		Point ko_;
		int white_captured_;
		int black_captured_;

		std::vector<StoneType> board_;
		std::vector<int> liberty_count_;
		std::vector<PointSet> liberties_;
		std::vector<PointSet> groups_;

		PointArr history_;
		PointArr handicap_;
	};
}
