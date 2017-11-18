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
