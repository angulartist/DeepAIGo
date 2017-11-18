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
        const int Threads = 4; //! 사용 스레드 개수

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