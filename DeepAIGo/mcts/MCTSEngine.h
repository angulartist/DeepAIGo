#pragma once

#include "../net/PolicyNet.h"
#include "TreeNode.h"

namespace DeepAIGo
{
    struct PolicyParams
    {
        Board board;
        TreeNode::Ptr node;

        PolicyParams(const Board& b, TreeNode::Ptr n) : board(b.Copy()), node(n) { }
    };

    class MCTSEngine
    {
    public:
        const int Threads = 4;

    public:
        MCTSEngine();

        Point GenMove(const Board& board);
        void DoMove(const Point& pt);

    private:
        void playout(const Board& board);
        void evaluate();

        void enqueue_policy(const Board& board, TreeNode::Ptr node);

    private:
        PolicyNet::Ptr policy_;
        
        TreeNode::Ptr root_;

        std::mutex mutex_;
        std::atomic<bool> stop_think_;

        std::deque<PolicyParams> policy_que_;
        std::atomic<int> policy_que_cnt_;
    };
}