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

#include "MCTSEngine.h"

#include <ctime>
#include <random>
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
            root_->Expand(policy_->EvalState(board, 0));
        }

        stop_think_ = false;

        std::vector<std::thread> threads(Threads);
        threads[0] = std::thread(&MCTSEngine::evaluate, this);

        for (int i = 0; i < 1; ++i)
        {
            for (int j = 1; j < Threads; ++j)
            {
                threads[j] = std::thread(&MCTSEngine::playout, this, std::ref(board));
            }

            for (int j = 1; j < Threads; ++j)
            {
                threads[j].join();
            }
        }
        stop_think_ = true;

        for (int j = 0; j < Threads; ++j)
            if (threads[j].joinable())
                threads[j].join();

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
            if (cpy.IsEnded()) break;
            node = node->Select();
            cpy.DoMove(node->GetAction());
        }

        float w = ((cpy.GetCurrentPlayer() == StoneType::BLACK && cpy.GetTrompTaylorScore() > 0) ||
                    (cpy.GetCurrentPlayer() == StoneType::WHITE && cpy.GetTrompTaylorScore() < 0)) ? 1 : -1;

        node->Update(w);
        enqueue_policy(cpy, node);
    }

    void MCTSEngine::evaluate()
    {
        while(1)
        {
            if (policy_que_cnt_ > 0)
            {
                std::lock_guard<std::mutex> lock(mutex_);

                for (int i = 0; i < policy_que_.size(); ++i)
                {
                    policy_que_[i].node->Expand(policy_->EvalState(
                        policy_que_[i].board, 0));
                }

                policy_que_.clear();
                policy_que_cnt_ = 0;
            }

            if (stop_think_) break;
        }
    }

    void MCTSEngine::enqueue_policy(const Board& board, TreeNode::Ptr node)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        policy_que_.emplace_back(board, node);
        atomic_fetch_add(&policy_que_cnt_, 1);
    }
}