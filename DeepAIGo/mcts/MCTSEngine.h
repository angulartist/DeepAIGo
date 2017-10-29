#pragma once

#include "../net/PolicyNet.h"
#include "TreeNode.h"

namespace DeepAIGo
{
    class MCTSEngine
    {
    public:
        MCTSEngine();

        Point GenMove(const Board& board);
        void DoMove(const Point& pt);

    private:
        void playout(const Board& board);

    private:
        PolicyNet::Ptr policy_;
        
        TreeNode::Ptr root_;
    };
}