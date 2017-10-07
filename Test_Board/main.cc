#include <iostream>
#include <board/Board.h>

using namespace DeepAIGo;

int main()
{
	Board board;

	board.DoMove("Q16"_pt); board.DoMove("D4"_pt); board.DoMove("Q3"_pt); board.DoMove("D16"_pt);
	board.DoMove("F17"_pt); board.DoMove("C14"_pt); board.DoMove("D18"_pt); board.DoMove("C17"_pt);
	board.DoMove("J17"_pt); board.DoMove("Q5"_pt); board.DoMove("R5"_pt); board.DoMove("R6"_pt);
	board.DoMove("R4"_pt);	board.DoMove("R14"_pt); board.DoMove("Q6"_pt); board.DoMove("Q7"_pt);
	board.DoMove("P6"_pt);	board.DoMove("R8"_pt); board.DoMove("S6"_pt);	board.DoMove("S7"_pt);
	board.DoMove("R7"_pt);	board.DoMove("S8"_pt);	board.DoMove("R6"_pt);

	board.ShowBoard();

	board.ShowLiberties();
	board.ShowGroups();

	Point last = board.GetHistory().back();
	const std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";

	std::cout << std::endl << "[Information]" << std::endl
		<< "Last Move: " << board.GetHistory().size() << " (" << coord[last.X] << ", " << last.Y + 1 << ")" << std::endl
		<< "Black Captured: " << board.GetBlackCaptured() << std::endl
		<< "White Captured: " << board.GetWhiteCaptured();

	std::cin.get();
}