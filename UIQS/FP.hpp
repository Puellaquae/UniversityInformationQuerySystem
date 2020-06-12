#pragma once
#include <functional>

template<typename R, typename ...As>
struct FunctionTraitsBase
{
	using function_type = std::function<R(As...)>;
	using result_type = R;
	using argument_types = std::tuple<As...>;
};

template<typename F>
struct FunctionTraits;
template<typename F>
struct FunctionTraits<std::reference_wrapper<F>> : FunctionTraits<F> {};
template<typename R, typename ...As>
struct FunctionTraits<R(*)(As...)> : FunctionTraitsBase<R, As...> {};
template<typename R, typename C, typename ...As>
struct FunctionTraits<R(C::*)(As...)> : FunctionTraitsBase<R, As...> {};
template<typename R, typename C, typename ...As>
struct FunctionTraits<R(C::*)(As...) const> : FunctionTraitsBase<R, As...> {};
template<typename F>
struct FunctionTraits : FunctionTraits<decltype(&F::operator())> {};

template<typename Itor, typename TProc, typename TResult = typename FunctionTraits<TProc>::result_type>
TResult Reduce(Itor begin, Itor end, TProc proc)
{
	TResult res{};
	for (; begin != end; ++begin)
	{
		res += proc(*begin);
	}
	return res;
}