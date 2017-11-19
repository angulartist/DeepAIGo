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

#pragma once

#include "../net/PolicyNet.h"

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <tuple>

namespace DeepAIGo
{
    //! 탐색 트리 노드
    class TreeNode : public std::enable_shared_from_this<TreeNode>
    {
    public:
        using Ptr = std::shared_ptr<TreeNode>;

        //! Virtual Loss
        static constexpr int Vl = 3;
        //! UCT 상수
        static constexpr float puct = 5;

    public:
        /**
         * @param action 유효한 행동
         * @param P 행동의 확률값
         * @param parent 부모 노드
         **/
        TreeNode(Point action = Pass, float P = 1.0f, Ptr parent = nullptr);

        //! 가장 적합한 노드를 선택합니다.
        TreeNode::Ptr Select();
        /** 노드를 확장합니다.
         * @param probs 유효 행동과 해당 확률들
         **/
        void Expand(const std::vector<ActionProb>& probs);

        //! 현재 노드의 행동을 구합니다.
        const Point& GetAction() const;
        /** 부모 노드를 설정합니다.
         * @param parent 새로운 부모 노드
         **/
        void SetParent(Ptr parent);
        //! 부모 노드를 구합니다.
        Ptr GetParent();
        //! 자식 노드를 구합니다.
        std::deque<Ptr>& GetChildren();

        //! 최상위 노드인지 확인합니다.
        bool IsRoot() const;
        //! 자식 노드의 여부를 확인합니다.
        bool HasChild() const;
    
        //! 노드의 방문 횟수를 구합니다.
        int GetVisits() const;
        //! 노드의 가치를 계산합니다.
        float GetValue() const;
        /** 노드의 값을 회귀적으로 갱신합니다.
         * @param w 이겼는지 여부
         * @param own 자신의 순서에 맞는 노드인지 여부
         **/
        void Update(int w, bool own = true);

    private:
        /** 현재 노드의 값을 갱신합니다.
         * @param w 이겼는지 여부
         * @param own 자신의 순서에 맞는 노드인지 여부
         **/
        void update(int w, bool own);

    private:
        //! 부모 노드
        Ptr parent_;
        //! 자식 노드
        std::deque<Ptr> children_;

        //! 노드의 행동
        Point action_;
        //! 노드의 확률
        std::atomic<float> P_;
        //! 이긴 횟수
        std::atomic<int> W_;
        //! 방문 횟수
        std::atomic<int> N_;

        //! 확장된 노드인지 여부
        bool is_expanded_;
        //! 노드 확장 뮤텍스
        std::mutex mutex_;
    };
}