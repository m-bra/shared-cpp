#ifndef FUNCTION_TRAITS_HPP_INCLUDED
#define FUNCTION_TRAITS_HPP_INCLUDED

#include <cstddef>
#include <tuple>

template <typename T>
struct FunctionTraits
    : public FunctionTraits<decltype(&T::operator())>
{};

template <typename C, typename R, typename... A>
struct FunctionTraits<R (C::*)(A...) const>
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

#endif
