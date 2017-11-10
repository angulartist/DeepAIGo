#pragma once

#include "helper.h"
#include "../preprocess/Preprocess.h"

#include <memory>

namespace DeepAIGo
{
    class ValueNet
    {
    public:
        using Ptr = std::shared_ptr<ValueNet>;

    public:
        ValueNet();
        virtual ~ValueNet();

        float EvalState(const Board& board);

        virtual void InitNetwork();

    private:
        mxnet::cpp::Symbol net_;
        Preprocess process_;

        mxnet::cpp::Executor* exec_;
        std::map<std::string, mxnet::cpp::NDArray> args_;
    };
}