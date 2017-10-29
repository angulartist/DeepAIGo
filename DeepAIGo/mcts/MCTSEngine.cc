#include "MCTSEngine.h"

#include <thread>

namespace DeepAIGo
{
    MCTSEngine::MCTSEngine()
    {
        policy_ = std::make_shared<PolicyNet>();
        policy_->InitNetwork();

        root_ = std::make_shared<TreeNode>();
    }

    Point MCTSEngine::GenMove(const Board& board)
    {
        if (!root_->HasChild())
        {
            root_->Expand(board, policy_);
        }

        const int thread = 4;

        for (int i = 0; i < 25; ++i)
        {
            std::vector<std::thread> threads(thread);

            for (int j = 0; j < thread; ++j)
            {
                threads[j] = std::thread(&MCTSEngine::playout, this, std::ref(board));
            }

            for (int j = 0; j < thread; ++j)
            {
                threads[j].join();
            }
        }

        root_ = root_->Select();
        return root_->GetAction();
    }

    void MCTSEngine::DoMove(const Point& pt)
    {
        auto it = std::find_if(root_->GetChildren().begin(), root_->GetChildren().end(), [&](TreeNode::Ptr node) {
            return node->GetAction() == pt; 
            });
        if (it != root_->GetChildren().end())
        {
            root_ = *it;
            root_->SetParent(nullptr);
        }
        else
        {
            root_ = std::make_shared<TreeNode>(pt);
        }
    }

    void MCTSEngine::playout(const Board& board)
    {
        auto node = root_;
        Board cpy = board.Copy();

        while (node->HasChild())
        {
            node = node->Select();
            cpy.DoMove(node->GetAction());
        }

        float w = ((board.GetCurrentPlayer() == StoneType::BLACK && cpy.GetTrompTaylorScore() > 0) ||
                    (board.GetCurrentPlayer() == StoneType::WHITE && cpy.GetTrompTaylorScore() < 0)) ? 1 : -1;

        node->Update(w);
        node->Expand(cpy, policy_);
    }
}