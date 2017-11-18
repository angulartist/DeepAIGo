#pragma once

#include "Common.h"

namespace DeepAIGo
{
	//! 바둑판 클래스
	class Board
	{
	public:
		Board();
		virtual ~Board() = default;

		//! 바둑판을 초기화합니다.
		void Clear();
		/** 현재 바둑판을 복사합니다.
		 * @return 복사된 바둑판
		 **/
		Board Copy() const;

		/** 유효한 착점인 지(바둑 규칙에 맞는 지) 확인
		 * @return 착점의 유효성
		 * @param pt 착점 좌표
		 **/
		bool IsValidMove(const Point& pt) const;
		/** 바둑판의 경계인지 확인합니다.
		 * @return 바둑판 경계 여부
		 * @param pt 착점 좌표
		 **/
		bool IsBoundary(const Point& pt) const;
		/** 바둑판 위의 좌표인지 확인합니다.
		 * @return 바둑판 위의 착점 여부
		 * @param pt 착점 좌표
		 **/
		bool IsOnBoard(const Point& pt) const;
		/** 자충수인지 확인합니다.
		 * @return 자충수 여부
		 * @param pt 착점 좌표
		 **/
		bool IsSuicide(const Point& pt) const;

		/** 소유자의 눈 모양인지 확인합니다.
		 * @return 눈 모양 여부
		 * @param pt 착점 좌표
		 * @param owner 소유자
		 **/
		bool IsEyeShape(const Point& pt, StoneType owner) const;
		/** 진짜 눈인지 확인합니다.
		 * @return 진짜 눈 모양 여부
		 * @param pt 착점 좌표
		 * @param owner 소유자
		 **/
		bool IsTrueEye(const Point& pt, StoneType owner) const;
		/** 눈의 소유자를 구합니다.
		 * @return 눈의 소유자. (소유자가 없으면 StoneType::EMPTY)
		 * @param pt 착점 좌표
		 * @param owner 소유자
		 **/
		StoneType GetEyeColor(const Point& pt) const;

		//! 게임이 종료되었는지 확인합니다.
		bool IsEnded() const;
		/** 해당 지점의 바둑돌 색상을 구합니다. (바둑돌이 없으면 StoneType::EMPTY)
		 * @return 바둑돌 색상
		 * @param pt 착점 좌표
		 **/
		StoneType GetStoneColor(const Point& pt) const;
		//! 현재 착수할 순서를 구합니다.
		StoneType GetCurrentPlayer() const;
		//! 현재 착수할 순서의 상대방을 구합니다.
		StoneType GetOpponentPlayer() const;

		//! 잡힌 흑돌의 개수를 구합니다.
		int GetBlackCaptured() const;
		//! 잡힌 백돌의 개수를 구합니다.
		int GetWhiteCaptured() const;
		//! 게임의 진행된 순서를 구합니다.
		const PointArr& GetHistory() const;
		//! 접바둑 돌의 위치를 구합니다.
		const PointArr& GetHandicap() const;

		/** 상/하/좌/우 좌표를 구합니다.
		 * @return 상/하/좌/우 좌표
		 * @param pt 착점 좌표
		 **/
		PointArr GetNeighbor(const Point& pt) const;
		/** 대각선 위치의 좌표를 구합니다.
		 * @return 대각선 위치 좌표
		 * @param pt 착점 좌표
		 **/
		PointArr GetDiagonal(const Point& pt) const;
		
		/** 활로를 구합니다.
		 * @return 해당 돌의 활로
		 * @param pt 돌의 좌표
		 **/
		const PointSet& GetLiberties(const Point& pt) const;
		/** 이어진 돌들의 무리를 구합니다.
		 * @return 이어진 돌들
		 * @param pt 돌의 좌표
		 **/
		const PointSet& GetGroup(const Point& pt) const;
		/** 주변의 돌들의 무리를 구합니다.
		 * @return 주변의 돌무리
		 * @param pt 착점 좌표
		 **/
		std::vector<PointSet> GetNeighborGroups(const Point& pt) const;
		/** 활로의 개수를 구합니다.
		 * @return 활로의 개수
		 * @param pt 돌의 좌표
		 **/
		int GetLibertyCount(const Point& pt) const;
		//! 유효 착점들을 구합니다.
		PointArr GetLegalMoves() const;

		/** 접바둑 돌을 놓습니다.
		 * @param pt 접바둑 돌 좌표
		 **/
		void PlaceHandicap(const Point& pt);
		/** 착수합니다.
		 * @param pt 착점 위치
		 * @param color 돌의 색상
		 * @param history true로 설정시 기록에 남김
		 **/
		void DoMove(const Point& pt, StoneType color, bool history = true);
		/** 착수합니다.
		 * 현재 순서에 따라 돌의 색상이 결정되며 기록에 남습니다.
		 * @param pt 착점 위치
		 **/
		void DoMove(const Point& pt);

		//! TrompTaylor 규칙에 따른 점수를 구합니다.
		float GetTrompTaylorScore() const;

		//! 바둑판을 출력합니다.
		void ShowBoard() const;
		//! 영토를 출력합니다.
		void ShowTerritory() const;
		//! 활로의 개수를 출력합니다.
		void ShowLibertyCount() const;
		//! 바둑판을 문자열로 내보냅니다.
		std::string ToString() const;

	private:
		/** 주변 돌 상태를 갱신합니다.
		 * @param pt 착점 위치
		 **/
		void update_neighbor(const Point& pt);
		/** 돌 무리를 제거합니다.
		 * @param group 돌 무리 집합
		 **/
		void remove_group(PointSet group);
		/** TrompTaylor 규칙에 따른 영역을 계산합니다.
		 * @return 함수의 재호출 필요 여부
		 * @param territory 영역
		 **/
		bool territory_floodfill(std::vector<int>& territory) const;

	private:
		//! 현재 순서의 색상
		StoneType current_player_;
		//! 게임이 끝났는지 여부
		bool is_ended_;
		//! 패의 위치
		Point ko_;
		//! 잡힌 백돌 개수
		int white_captured_;
		//! 잡힌 흑돌 개수
		int black_captured_;

		//! 바둑판 정보
		std::vector<StoneType> board_;
		//! 활로 개수
		std::vector<int> liberty_count_;
		//! 활로
		std::vector<PointSet> liberties_;
		//! 돌 무리
		std::vector<PointSet> groups_;

		//! 게임 기록
		PointArr history_;
		//! 접바둑 돌
		PointArr handicap_;
	};
}
