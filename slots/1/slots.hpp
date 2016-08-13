#ifndef SLOTS_HPP_INCLUDED
#define SLOTS_HPP_INCLUDED

#include "Optional.hpp"
#include "Iterator.hpp"
#include <vector>

template <typename T>
struct OptNotEmpty
{ bool operator()(Optional<T> *opt) {return !opt->empty;}};
template <typename T>
struct UnwrapOpt
{   typedef T *Result;
    Result operator()(Optional<T> *opt) {
	return &opt->value();
    }
};

template <typename T>
using SlotIterator = Iterator<
    MapIterator<UnwrapOpt<T>, FilterIterator<OptNotEmpty<T>, RawIterator<Optional<T>>>>>;

template <typename T>
struct ConstOptNotEmpty
{ bool operator()(Optional<T> const *opt) {return !opt->empty;}};
template <typename T>
struct ConstUnwrapOpt
{   typedef T const *Result;
    Result operator()(Optional<T> const *opt) {
        return &opt->value;
    }
};

template <typename T>
using ConstSlotIterator = Iterator<
    MapIterator<ConstUnwrapOpt<T>, FilterIterator<ConstOptNotEmpty<T>, RawIterator<const Optional<T>>>>>;

template <typename T>
using Slot = Optional<T>;

template <typename T, std::size_t N>
struct Slots
{
    Optional<T> _slots[N];

    SlotIterator<T> iter()
    {
	return ::iter(_slots, N)
	    .filter(OptNotEmpty<T>())
	    .map(UnwrapOpt<T>());
    }

    ConstSlotIterator<T> iter() const
    {
	return ::iter(_slots, N)
	    .filter(ConstOptNotEmpty<T>())
	    .map(ConstUnwrapOpt<T>());
    }

    Iterator<FilterIterator<OptNotEmpty<T>, RawIterator<Optional<T>>>>
	iter_nonempty_slots()
    {
	return ::iter(_slots, N)
	    .filter(OptNotEmpty<T>());
    }

    Slot<T> *add(T const &elt)
    {
	for (auto it = _slots; it != _slots + N; ++it)
	    if (it->empty)
	    {
		*it = Optional<T>(elt);
		return {&*it};
	    }
        assert(false);
	return 0;
    }

    bool contains_slot(Slot<T> *id) const
    {
	return id > _slots.data() && id < _slots.data() + _slots.size();
    }
};

#endif
