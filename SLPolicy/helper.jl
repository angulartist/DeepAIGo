using MXNet

function ConvFactory(data, num_filter, kernel; stride=(1,1), pad=(1,1), act_type=:relu)
	conv = mx.Convolution(data, num_filter=num_filter, kernel=kernel, stride=stride, pad=pad)
	bn = mx.BatchNorm(conv)
	act = mx.Activation(bn, act_type=act_type)

	return act
end