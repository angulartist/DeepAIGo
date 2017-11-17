#pragma once

#include "helper.h"
#include "../preprocess/Preprocess.h"

#include <memory>

namespace DeepAIGo
{
    //! Value Network
    class ValueNet
    {
    public:
        using Ptr = std::shared_ptr<ValueNet>;

    public:
        ValueNet();
        virtual ~ValueNet();

        /** Value Network를 연산합니다.
         * @param board 바둑판 상태
         **/
        float EvalState(const Board& board);

        //! 신경망을 초기화 합니다.
        virtual void InitNetwork();

    private:
        mxnet::cpp::Symbol net_;    //! 신경망
        Preprocess process_;        //! 전처리기

        mxnet::cpp::Executor* exec_;                        //! Executor
        std::map<std::string, mxnet::cpp::NDArray> args_;   //! 신경망 파라메터 목록
    };
}