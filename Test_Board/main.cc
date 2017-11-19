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

	Point last = board.GetHistory().back();
	const std::string coord = "ABCDEFGHJKLMNOPQRSTUVWXYZ";

	std::cout << std::endl << "[Information]" << std::endl
		<< "Last Move: " << board.GetHistory().size() << " (" << coord[last.X] << ", " << last.Y + 1 << ")" << std::endl
		<< "Black Captured: " << board.GetBlackCaptured() << std::endl
		<< "White Captured: " << board.GetWhiteCaptured();

	std::cin.get();
}