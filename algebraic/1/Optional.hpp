#ifndef OPTIONAL_HPP_INCLUDED
#define OPTIONAL_HPP_INCLUDED

#include <cassert>
#include <new>

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
	return _value;
    }

    T const &value() const
    {
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


#endif
