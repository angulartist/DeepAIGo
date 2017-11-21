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

#include "TreeNode.h"

namespace DeepAIGo
{
    TreeNode::TreeNode(Point action, float P, TreeNode::Ptr parent)
        : action_(action), P_(P), parent_(parent), is_expanded_(false)
    {
        W_ = 0;
        N_ = 0;
    }

    TreeNode::Ptr TreeNode::Select()
    {
        size_t total_games = 0;
        for (auto& n : children_)
            total_games += n->GetVisits();

        size_t max_idx = 0;
        float max_value = -9999.f;

        for (size_t i = 0; i < children_.size(); ++i)
        {
            float q = (GetVisits()) ? W_.load() / GetVisits() : 0;
            float u = TreeNode::puct * children_[i]->P_.load() * std::sqrt(total_games) / (1.f + GetVisits());

            if (q + u > max_value)
            {
                max_value = q + u;
                max_idx = i;
            }
        }

        return children_[max_idx];
    }

    TreeNode::Ptr TreeNode::Play(float temperature)
    {
        float sum = 0;
        for (auto& n : children_)
            sum += std::pow(n->GetVisits(), 1/temperature);

        size_t max_idx = 0;
        float max_value = -9999.f;

        for (size_t i = 0; i < children_.size(); ++i)
        {
            float value = std::pow(children_[i]->GetVisits(), 1/temperature);

            if (max_value < value)
            {
                max_value = value;
                max_idx = i;
            }
        }

        return children_[max_idx];
    }

    void TreeNode::Expand(const std::vector<ActionProb>& probs)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (is_expanded_) return;

        is_expanded_ = true;

        for (auto& p : probs)
        {
            children_.emplace_back(
                std::make_shared<TreeNode>(std::get<0>(p), std::get<1>(p), shared_from_this()));
        }
    }

    const Point& TreeNode::GetAction() const
    {
        return action_;
    }

    float TreeNode::GetPrior() const
    {
        return P_.load();
    }

    void TreeNode::SetParent(TreeNode::Ptr parent)
    {
        parent_ = parent;
    }

    TreeNode::Ptr TreeNode::GetParent()
    {
        return parent_;
    }

    std::deque<TreeNode::Ptr>& TreeNode::GetChildren()
    {
        return children_;
    }

    bool TreeNode::IsRoot() const
    {
        return parent_ == nullptr;
    }

    bool TreeNode::HasChild() const
    {
        return children_.size() > 0;
    }

    int TreeNode::GetVisits() const
    {
        return N_.load();
    }

    float TreeNode::GetQValue() const
    {
        float q = (GetVisits()) ? W_.load() / GetVisits() : 0;

        return q;
    }

    void TreeNode::Update(int w, bool own)
    {
        if (parent_ != nullptr)
            parent_->Update(w, !own);

        update(w, own);
    }

    void TreeNode::update(int w, bool own)
    {
        atomic_fetch_add(&N_, 1);
        atomic_fetch_add(&W_, w * (own ? 1 : -1));
        atomic_fetch_sub(&W_, TreeNode::Vl);
    }
}