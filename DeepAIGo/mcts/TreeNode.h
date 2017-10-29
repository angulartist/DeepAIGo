#pragma once

#include "../net/PolicyNet.h"

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <tuple>

namespace DeepAIGo
{
    class TreeNode : public std::enable_shared_from_this<TreeNode>
    {
    public:
        using Ptr = std::shared_ptr<TreeNode>;

        static constexpr int Vl = 3; // Virtual loss
        static constexpr float puct = 5;

    public:
        TreeNode(Point action = Pass, float P = 1.0f, Ptr parent = nullptr);

        TreeNode::Ptr Select();
        void Expand(const Board& board, PolicyNet::Ptr policy);

        const Point& GetAction() const;
        void SetParent(Ptr parent);
        Ptr GetParent();
        std::deque<Ptr>& GetChildren();

        bool IsRoot() const;
        bool HasChild() const;
    
        int GetVisits() const;
        float GetValue() const;
        void Update(int w);

    private:
        void update(int w);

    private:
        Ptr parent_;
        std::deque<Ptr> children_;

        Point action_;
        std::atomic<float> P_;
        std::atomic<int> W_;
        std::atomic<int> N_;

        bool is_expanded_;
        std::mutex mutex_;
    };
}