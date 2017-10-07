#include <io/SgfParser.h>
#include <iostream>

int main()
{
	auto board = DeepAIGo::SgfParser::LoadFromFile("Test.sgf");

	board.ShowBoard();

	DeepAIGo::Point last = board.GetHistory().back();
	const std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";

	std::cout << std::endl << "[Information]" << std::endl
		<< "Last Move: " << board.GetHistory().size() << " (" << coord[last.X] << ", " << last.Y + 1 << ")" << std::endl
		<< "Black Captured: " << board.GetBlackCaptured() << std::endl
		<< "White Captured: " << board.GetWhiteCaptured();

	std::cin.get();
}