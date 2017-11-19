#= MIT License

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
SOFTWARE.=#

using HDF5
using MXNet

include("helper.jl")

type PolicyNet
    net
    model
end

function CreatePolicyNet(prefix::String="", epoch::Int=0)
    data = mx.Variable(:data)
	
	layers = Array{mx.SymbolicNode,1}()
	push!(layers, ConvFactory(data, 128, (5, 5), pad=(2, 2)))

	for i = 2:12
		conv = ConvFactory(layers[end], 128, (3, 3))

		push!(layers, conv)
	end

	conv = mx.Convolution(layers[end], kernel=(1, 1), num_filter=1, pad=(0, 0))
	flatten = mx.Flatten(conv)
	
	softmax = mx.SoftmaxOutput(flatten, name=:softmax)
	if prefix == ""
		model = mx.FeedForward(softmax, context=mx.gpu())
	else
		model = mx.load_checkpoint(prefix, epoch, mx.FeedForward, context=mx.gpu())
	end

	PolicyNet(softmax, model)
end
