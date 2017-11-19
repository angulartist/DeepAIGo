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

#include <iostream>
#include <boost/multi_array.hpp>
#include <board/Board.h>

using namespace DeepAIGo;

void ShowTurnSince(Board board)
{
	boost::multi_array<int, 2> ret{ boost::extents[BOARD_SIZE][BOARD_SIZE] };
	std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);
	
	if (board.GetHistory().size() != 0)
	for (int i = 0; i < std::min((int)board.GetHistory().size(), 8); ++i)
	{
		int idx = std::max((int)board.GetHistory().size() - (i + 1), 0);
		auto& history = board.GetHistory()[idx];

		if (history != Pass || history != Resign)
			ret[history.Y][history.X] = i + 1;
	}

	printf("   ");
	std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	for (size_t i = 0; i < BOARD_SIZE; ++i)
		printf("%c ", coord[i]);

	printf("\n");
	for (int y = BOARD_SIZE - 1; y > 0; --y)
	{
		printf("%02d", (int)y + 1);
		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			printf(" %d", ret[y][x]);
		}
		printf("\n");
	}
}

void ShowLiberties(Board board)
{
	boost::multi_array<int, 2> ret{ boost::extents[BOARD_SIZE][BOARD_SIZE] };
	std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

	for (size_t x = 0; x < BOARD_SIZE; ++x)
	{
		for (size_t y = 0; y < BOARD_SIZE; ++y)
		{
			auto pt = Point(x, y);

			if (board.GetLibertyCount(pt) > 0)
				ret[y][x] = std::min(board.GetLibertyCount(pt), 8);
		}
	}

	printf("   ");
	std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	for (size_t i = 0; i < BOARD_SIZE; ++i)
		printf("%c ", coord[i]);

	printf("\n");
	for (int y = BOARD_SIZE - 1; y > 0; --y)
	{
		printf("%02d", (int)y + 1);
		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			printf(" %d", ret[y][x]);
		}
		printf("\n");
	}
}

void ShowCaptureSize(Board board)
{
	boost::multi_array<int, 2> ret{ boost::extents[BOARD_SIZE][BOARD_SIZE] };
	std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

	for (auto& m : board.GetLegalMoves())
	{
		int captured = 0;

		for (auto& n : board.GetNeighborGroups(m))
		{
			if (n.size() == 0) continue;

			if (board.GetLibertyCount(*n.begin()) == 1 && board.GetStoneColor(*n.begin()) == board.GetOpponentPlayer())
				captured += n.size();
		}

		if (captured > 0)
		{
			ret[m.Y][m.X] = std::min(captured, 8);
		}
	}

	printf("   ");
	std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	for (size_t i = 0; i < BOARD_SIZE; ++i)
		printf("%c ", coord[i]);

	printf("\n");
	for (int y = BOARD_SIZE - 1; y > 0; --y)
	{
		printf("%02d", (int)y + 1);
		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			printf(" %d", ret[y][x]);
		}
		printf("\n");
	}
}

void ShowSelfAtariSize(Board board)
{
	boost::multi_array<int, 2> ret{ boost::extents[BOARD_SIZE][BOARD_SIZE] };
	std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

	for (auto& m : board.GetLegalMoves())
	{
		auto liberties = board.GetLiberties(m);

		PointSet groups, captured;
		groups.insert(m);

		for (auto& n : board.GetNeighborGroups(m))
		{
			if (n.size() == 0) continue;

			auto& g = board.GetGroup(*n.begin());

			if (board.GetStoneColor(*n.begin()) == board.GetCurrentPlayer())
			{
				auto& li = board.GetLiberties(*n.begin());
				liberties.insert(li.begin(), li.end());
				groups.insert(g.begin(), g.end());
			}
			else if (board.GetLibertyCount(*n.begin()) == 1)
			{
				captured.insert(g.begin(), g.end());
			}
		}

		if (captured.size() > 0)
		{
			for (auto& g : groups)
			{
				for (auto& n : board.GetNeighbor(g))
				{
					if (std::find(captured.begin(), captured.end(), n) != captured.end())
						liberties.insert(n);
				}
			}
		}

		auto it = std::find(liberties.begin(), liberties.end(), m);
		if (it != liberties.end())
			liberties.erase(it);

		if (liberties.size() == 1)
			if (groups.size() > 0)
				ret[m.Y][m.X] = std::min((int)groups.size(), 8);
	}

	printf("   ");
	std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	for (size_t i = 0; i < BOARD_SIZE; ++i)
		printf("%c ", coord[i]);

	printf("\n");
	for (int y = BOARD_SIZE - 1; y > 0; --y)
	{
		printf("%02d", (int)y + 1);
		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			printf(" %d", ret[y][x]);
		}
		printf("\n");
	}
}

void ShowLibertiesAfterMove(Board board)
{
	boost::multi_array<int, 2> ret{ boost::extents[BOARD_SIZE][BOARD_SIZE] };
	std::fill(ret.origin(), ret.origin() + ret.num_elements(), 0);

	for (auto& m : board.GetLegalMoves())
	{
		auto liberties = board.GetLiberties(m);

		PointArr groups, captured;
		groups.push_back(m);

		for (auto& n : board.GetNeighborGroups(m))
		{
			if (n.size() == 0) continue;

			auto& g = board.GetGroup(*n.begin());

			if (board.GetStoneColor(*n.begin()) == board.GetCurrentPlayer())
			{
				auto& li = board.GetLiberties(*n.begin());
				liberties.insert(li.begin(), li.end());
				groups.insert(groups.end(), g.begin(), g.end());
			}
			else if (board.GetLibertyCount(*n.begin()) == 1)
			{
				captured.insert(captured.end(), g.begin(), g.end());
			}
		}

		if (captured.size() > 0)
		{
			for (auto& g : groups)
			{
				for (auto& n : board.GetNeighbor(g))
				{
					if (std::find(captured.begin(), captured.end(), n) != captured.end())
						liberties.insert(n);
				}
			}
		}

		auto it = std::find(liberties.begin(), liberties.end(), m);
		if (it != liberties.end())
			liberties.erase(it);

		if (liberties.size() > 0)
			ret[m.Y][m.X] = std::min((int)liberties.size(), 8);
	}

	printf("   ");
	std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	for (size_t i = 0; i < BOARD_SIZE; ++i)
		printf("%c ", coord[i]);

	printf("\n");
	for (int y = BOARD_SIZE - 1; y > 0; --y)
	{
		printf("%02d", (int)y + 1);
		for (size_t x = 0; x < BOARD_SIZE; ++x)
		{
			printf(" %d", ret[y][x]);
		}
		printf("\n");
	}
}

int main()
{
	Board board;

	board.DoMove("Q16"_pt); board.DoMove("D4"_pt); board.DoMove("Q3"_pt); board.DoMove("D16"_pt);
	board.DoMove("F17"_pt); board.DoMove("C14"_pt); board.DoMove("D18"_pt); board.DoMove("C17"_pt);
	board.DoMove("J17"_pt); board.DoMove("Q5"_pt); board.DoMove("R5"_pt); board.DoMove("R6"_pt);
	board.DoMove("R4"_pt);	board.DoMove("R14"_pt); board.DoMove("Q6"_pt); board.DoMove("Q7"_pt);
	board.DoMove("P6"_pt);	board.DoMove("R8"_pt); board.DoMove("S6"_pt);	board.DoMove("S7"_pt);
	board.DoMove("R7"_pt);	board.DoMove("S8"_pt);	board.DoMove("Q14"_pt);

	board.ShowBoard();

	std::cout << std::endl << "[Turns Since]" << std::endl;
	ShowTurnSince(board);

	std::cout << std::endl << "[Liberties]" << std::endl;
	ShowLiberties(board);

	std::cout << std::endl << "[Capture Size]" << std::endl;
	ShowCaptureSize(board);

	std::cout << std::endl << "[Self Atari Size]" << std::endl;
	ShowSelfAtariSize(board);

	std::cout << std::endl << "[Liberties After Move]" << std::endl;
	ShowLibertiesAfterMove(board);

	std::cin.get();
}