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
#include "TreeNode.h"

namespace DeepAIGo
{
    //! Policy Network 입력 파라메터
    struct PolicyParams
    {
        Board board;        //! 바둑판
        TreeNode::Ptr node; //! 노드

        /**
         * @param b 바둑판
         * @param n 확장할 노드
         **/
        PolicyParams(const Board& b, TreeNode::Ptr n) : board(b.Copy()), node(n) { }
    };

    //! MonteCarlo TreeSearch 엔진
    class MCTSEngine
    {
    public:
        MCTSEngine();

        /** 착수 위치를 결정합니다.
         * @return 착수 위치
         * @param board 바둑판 상태
         **/
        Point GenMove(const Board& board);
        /** 착수 위치를 입력합니다.
         * @param pt 착수 위치
         **/
        void DoMove(const Point& pt);

        //! 탐색 트리를 초기화합니다.
        void Clear();

    private:
        /** playout을 수행하는 스레드
         * @param board 바둑판 상태
         **/
        void playout(const Board& board);
        //! 신경망 연산 스레드
        void evaluate();

        /** Policy network 작업 큐에 등록합니다.
         * @param board 바둑판 상태
         * @param node 갱신할 노드
         **/
        void enqueue_policy(const Board& board, TreeNode::Ptr node);

    private:
        //! Policy Network
        PolicyNet::Ptr policy_;
        //! 최상위 노드
        TreeNode::Ptr root_;

        //! 뮤택스
        std::mutex mutex_;
        //! 생각을 멈춰야 하는지 여부
        std::atomic<bool> stop_think_;

        //! Policy network 작업 큐
        std::deque<PolicyParams> policy_que_;
        //! Policy network에 등록된 작업 개수
        std::atomic<int> policy_que_cnt_;
    };
}