#pragma once

#define BuildHasMemberQ(member)\
template <typename T>\
struct HasMember##member##Q\
{\
		template <typename _T>\
		static auto check(_T) -> typename std::decay<decltype(_T::##member)>::type;\
		static void check(...);\
		using type = decltype(check(std::declval<T>()));\
		enum { value = !std::is_void<type>::value };\
}

template<typename R, typename ...As>
struct FunctionTraitsBase
{
	using function_type = std::function<R(As...)>;
	using result_type = R;
	using argument_types = std::tuple<As...>;
};

template<typename F>
struct FunctionTraitsImpl;

template<typename F>
struct FunctionTraitsImpl<std::reference_wrapper<F>> : FunctionTraitsImpl<F> {};

template<typename R, typename ...As>
struct FunctionTraitsImpl<R(*)(As...)> : FunctionTraitsBase<R, As...> {};

template<typename R, typename C, typename ...As>
struct FunctionTraitsImpl<R(C::*)(As...)> : FunctionTraitsBase<R, As...> {};

template<typename R, typename C, typename ...As>
struct FunctionTraitsImpl<R(C::*)(As...) const> : FunctionTraitsBase<R, As...> {};

template<typename F>
struct FunctionTraitsImpl : FunctionTraitsImpl<decltype(&F::operator())> {};

template<typename F>
struct FunctionTraits : FunctionTraitsImpl<std::decay_t<F>> {};

template<bool A, bool B>
struct And {};

template<>
struct And<true, true>
{
	enum { value = true };
};

template<>
struct And<true, false>
{
	enum { value = false };
};

template<>
struct And<false, true>
{
	enum { value = false };
};

template<>
struct And<false, false>
{
	enum { value = false };
};

template<bool A, bool B>
constexpr bool and_v = And<A, B>::value;
