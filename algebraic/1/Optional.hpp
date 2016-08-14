#ifndef OPTIONAL_HPP_INCLUDED
#define OPTIONAL_HPP_INCLUDED

#include <cassert>
#include <new>
#include "FunctionTraits.hpp"

template<typename T>
struct Optional
{
    /// this has to be first so that Optional can be reinterpreted as T
    T _value;
    bool empty;

    /// ensure rest of the code that the value member is still first
    static constexpr bool value_is_first_member = true;

    Optional()
    {
	empty = true;
    }

    Optional(T const &value): _value(value)
    {
	empty = false;
    }

    T &value()
    {
	assert(!empty);
	return _value;
    }

    T const &value() const
    {
	assert(!empty);
	return _value;
    }

    T &value_or(T &replace_empty)
    {
	if (empty)
	{
	    return replace_empty;
	}
	else
	{
	    return value();
	}
    }
    
    T const &value_or(T const &replace_empty) const
    {
	if (empty)
	{
	    return replace_empty;
	}
	else
	{
	    return value();
	}
    }

    template <typename F>
    bool do_value(F const &func)
    {
	if (!empty)
	    func(value());
	return !empty;
    }

    template <typename F>
    Optional<typename FunctionTraits<F>::Result>
    map(F const &func) const
    {
	if (!empty)
	    return Optional<typename FunctionTraits<F>::Result>(func(value()));
	return Optional<typename FunctionTraits<F>::Result>();
    }

    T const &assert_value() const
    {
	assert(!empty);
	return value();
    }

    T &assert_value()
    {
	assert(!empty);
	return value();
    }
};

template<typename T>
struct Optional<T &>
{
    // references are no entities by themselves. use a pointer instead.
};

template <typename T>
inline Optional<T> optional(T const &t)
{
    return Optional<T>(t);
}

template <typename T>
inline Optional<T> empty()
{
    return Optional<T>();
}

#endif
