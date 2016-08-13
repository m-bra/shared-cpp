#ifndef SELECTOR_HPP_INCLUDED
#define SELECTOR_HPP_INCLUDED

#include "Optional.hpp"
#include <vector>

template <typename T>
struct RawIterator
{
    typedef T *Element;
    
    T *_begin;
    T const *_end;

    RawIterator(): _begin(0), _end(0) {}
    RawIterator(T *begin, T const *end): _begin(begin), _end(end) {}
    RawIterator(T *begin, std::size_t count): _begin(begin), _end(begin + count) {}

    Optional<T *> next()
    {
	if (_begin == _end)
	{
	    return Optional<T *>();
	}
	else
	{
	    return Optional<T *>(_begin++);
	}
    }
};

// F is a function: bool(Element &) or bool(Element) or bool (Element const &)
template <typename F, typename I>
struct FilterIterator
{
    typedef typename I::Element Element;
    I _iterator;
    F _predicate;

    FilterIterator(I iterator, F predicate): _iterator(iterator), _predicate(predicate) {}

    Optional<Element> next()
    {
       	while (true)
	{
	    Optional<Element> const &n = _iterator.next();
	    if (n.empty)
		break;
	    Element const &elt = n.value();
	    if (_predicate(elt))
		return Optional<Element>(elt);
	}
	return Optional<Element>();
    }
};

template <typename F>
struct MapFunction
{
    typedef typename F::Result Result;
};

template <typename R, typename T>
struct MapFunction<R (*)(T)>
{
    typedef R Result;
};

template <typename R, typename T>
struct MapFunction<R(T)>
{
    typedef R Result;
};

// F is a function: R(Element) or R(Element &) or R(Element const &)
template <typename F, typename I>
struct MapIterator
{
    typedef typename MapFunction<F>::Result Element;
    I _iterator;
    F _function;

    MapIterator(I iterator, F function): _iterator(iterator), _function(function) {}

    Optional<Element> next()
    {
	Optional<typename I::Element> &&n = _iterator.next();
	if (n.empty)
	    return Optional<Element>();
	return _function(n.value());
    }
};

template <typename I>
struct Iterator
{
    typedef typename I::Element Element;

    I _iterator;
    
    Iterator(I iterator): _iterator(iterator) {}

    Optional<Element> next()
    {
	return _iterator.next();
    }

    template <typename F, typename R>
    R fold(R start, F const &combine)
    {
	Optional<Element> &&n = next();
	if (n.empty)
	    return start;
	
	R &&c = combine(start, n.value());
	return fold(c, combine);
    }

    template <typename F>
    Iterator<MapIterator<F, I>> map(F f)
    {
	return Iterator<MapIterator<F, I>>(
	    MapIterator<F, I>(_iterator, f));
    }

    template <typename F>
    Iterator<FilterIterator<F, I>> filter(F p)
    {
	return Iterator<FilterIterator<F, I>>(
	    FilterIterator<F, I>(_iterator, p));
    }

    template <typename F>
    Iterator<I> do_each(F const &f)
    {
	while (true)
	{
	    Optional<Element> &&optelt = _iterator.next();
	    if (optelt.empty) break;
	    f(optelt.value());
	}
	return *this;
    }

    std::vector<Element> collect()
    {
	return std::move(
	    fold(std::vector<Element>(),
	     [](std::vector<Element> &vec, Element &elt)
	     {
		 vec.push_back(elt);
		 return std::move(vec);
	     }));
    }

    // consumes items and counts them.
    std::size_t count()
    {
	if (next().empty)
	    return 0;
	return 1 + count();
    }
};

template <typename T>
inline Iterator<RawIterator<T>> iter(T *begin, T const *end)
{
    return RawIterator<T>(begin, end);
}

template <typename T>
inline Iterator<RawIterator<T>> iter(T *begin, std::size_t count)
{
    return RawIterator<T>(begin, count);
}

template <typename T>
inline Iterator<RawIterator<T const>> iter(T const* begin, std::size_t count)
{
    return RawIterator<T const>(begin, count);
}

template <typename T>
inline Iterator<RawIterator<T>> iter(std::vector<T> &vec)
{
    return RawIterator<T>(vec.data(), vec.size());
}

template <typename T>
inline Iterator<RawIterator<T const>> iter(std::vector<T> const &vec)
{
    return RawIterator<T const>(vec.data(), vec.size());
}

#endif
