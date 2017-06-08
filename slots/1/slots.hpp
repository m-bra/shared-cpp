#ifndef SLOTS_HPP_INCLUDED
#define SLOTS_HPP_INCLUDED

#include "Optional.hpp"
#include "Iterator.hpp"
#include "Logger.hpp"
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
        LOG_WARNING("No more space for new slots. Killing oldest slots now.");
	for (auto it = _slots; it != _slots + (size_t)(0.3 * N); ++it)
	    it->empty = true;
	return add(elt);
    }

    bool contains_slot(Slot<T> *id) const
    {
	return id >= &_slots[0] && id < &_slots[N];
    }

    Slot<T> *slot_at(size_t i)
    {
	assert(i < N);
	return &_slots[i];
    }

    size_t slot_i(Slot<T> *slot)
    {
	assert(contains_slot(slot));
	return slot - slot_at(0);
    }
};

#endif
