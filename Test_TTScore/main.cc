#include <board/Board.h>
#include <io/SgfParser.h>

#include <stdio.h>

int main()
{
    DeepAIGo::Board board = DeepAIGo::SgfParser::LoadFromFile("test.sgf");

    board.ShowBoard();
    board.ShowTerritory();
    printf("\n%f", board.GetTrompTaylorScore());

    getchar();

    return 0;
}