#include <mcts/MCTSEngine.h>

using namespace DeepAIGo;

int main()
{
    Board board;
    MCTSEngine engine;

    for (int i = 0; i < 4; ++i)
    {
        auto pt = engine.GenMove(board);
        board.DoMove(pt);

        board.ShowBoard();
    }
}