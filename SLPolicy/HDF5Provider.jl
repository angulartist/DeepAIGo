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

using MXNet
using HDF5

type HDF5Provider <: mx.AbstractDataProvider
	data::HDF5Dataset
	label::HDF5Dataset
	
	batch_size::Int

	data_shape::Vector{Tuple{Symbol, Tuple}}
	label_shape::Vector{Tuple{Symbol, Tuple}}

	sample_count::Int
	real_samples::Int

	data_batch::Vector{mx.NDArray}
	label_batch::Vector{mx.NDArray}
end

mx.get_batch_size(provider::HDF5Provider) = provider.batch_size

mx.provide_data(provider::HDF5Provider) = provider.data_shape

mx.provide_label(provider::HDF5Provider) = provider.label_shape

function HDF5Provider(data::HDF5Dataset, label::HDF5Dataset;
		      batch_size::Int=0, shuffle::Bool=false,
		      data_name::Symbol=:data, label_name::Symbol=:softmax_label)
	data_shape = Tuple{Symbol, Tuple}[(data_name, (size(data[:, :, :, 1])[1:3]..., batch_size))]
	label_shape = Tuple{Symbol, Tuple}[(label_name::Symbol, (batch_size,))]

	sample_count = size(data)[4]
	real_samples = Int(floor(sample_count / batch_size)) * batch_size

	@assert 0 < batch_size <= sample_count

	data_batch = [mx.empty(data_shape[1][2][1:end-1]..., batch_size)]
	label_batch = [mx.empty(batch_size,)]

	HDF5Provider(data, label, batch_size, data_shape, label_shape, sample_count, real_samples, data_batch, label_batch)
end

immutable HDF5ProviderState <: mx.AbstractDataProviderState
	curr_idx::Int
end

immutable HDF5DataBatch <: mx.AbstractDataBatch
	index::UnitRange{Int}
end

function Base.eltype(provider::HDF5Provider)
	HDF5ProviderState
end

function Base.start(provider::HDF5Provider)
	HDF5ProviderState(1)
end

function Base.done(provider::HDF5Provider, state::HDF5ProviderState)
	state.curr_idx > provider.real_samples
end

function Base.next(provider::HDF5Provider, state::HDF5ProviderState)
	idx = state.curr_idx:Base.min(state.curr_idx + provider.batch_size-1, provider.real_samples)

	return (HDF5DataBatch(idx), HDF5ProviderState(idx.stop+1))
end

function mx.count_samples(provider::HDF5Provider, batch::HDF5DataBatch)
	return length(batch.index)
end

function mx.get_data(provider::HDF5Provider, batch::HDF5DataBatch)
	mx.copy!(provider.data_batch[1], provider.data[:, :, :, batch.index])

	return provider.data_batch
end

function mx.get_label(provider::HDF5Provider, batch::HDF5DataBatch)
	mx.copy!(provider.label_batch[1], provider.label[batch.index])

	return provider.label_batch
end
