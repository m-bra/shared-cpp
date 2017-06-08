#ifndef FUNCTION_TRAITS_HPP_INCLUDED
#define FUNCTION_TRAITS_HPP_INCLUDED

#include <cstddef>
#include <tuple>

template <typename T>
struct FunctionTraits
    : public FunctionTraits<decltype(&T::operator())>
{};

template <typename C, typename R, typename... A>
struct FunctionTraits<R (C::*)(A...)>
    : public FunctionTraits<R(A...)>
{};

template <typename C, typename R, typename... A>
struct FunctionTraits<R (C::*)(A...) const>
    : public FunctionTraits<R(A...)>
{};

template <typename R, typename... A>
struct FunctionTraits<R (*)(A...)>
    : public FunctionTraits<R(A...)>
{};

template <typename R, typename... A>
struct FunctionTraits<R(A...)>
{
    static constexpr size_t arity = sizeof...(A);
    // arity is the number of arguments.

    typedef R Result;

    template <size_t i>
    struct Arg
    {
        typedef typename std::tuple_element<i, std::tuple<A...>>::type Type;
    };
};

template <typename R, typename... A>
struct FunctionTraits<R (A...) const>
    : public FunctionTraits<R(A...)>
{};

#endif
