#pragma once
#include "Types.hpp"

template<typename TItor, typename TProc, typename TResult = typename FunctionTraits<TProc>::result_type>
std::enable_if_t<
	std::is_same_v<
		std::decay_t<std::tuple_element_t<0, typename FunctionTraits<TProc>::argument_types>>,
		typename TItor::value_type
	>,
	TResult
>
MapAndSum(TItor begin, TItor end, TProc proc)
{
	TResult res{};
	for (; begin != end; ++begin)
	{
		res += proc(*begin);
	}
	return res;
}

template<typename TItor, typename TProc, typename TValue = typename TItor::value_type>
std::enable_if_t<
	and_v<
		std::is_same_v<typename FunctionTraits<TProc>::result_type, TValue>,
		and_v<
			std::is_same_v<std::decay_t<std::tuple_element_t<0, typename FunctionTraits<TProc>::argument_types>>, TValue>,
			std::is_same_v<std::decay_t<std::tuple_element_t<1, typename FunctionTraits<TProc>::argument_types>>, TValue>
			>
		>,
	TValue>
	Reduce(TItor begin, TItor end, TProc proc)
{
	if (begin == end)return {};

	TValue res = *begin;
	for (++begin; begin != end; ++begin)
	{
		res = proc(res, *begin);
	}
	return res;
}