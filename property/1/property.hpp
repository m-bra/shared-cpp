
#include "FunctionTraits.hpp"

template <typename T, typename G, typename S>
class Property
{
    T _t;
    G get;
    S set;
public:
    Property(T const &start, G const &g, S const &s):
	_t(start), get(g), set(s) {}

    Property(G const &g, S const &s):
	get(g), set(s) {}
    
    typename FunctionTraits<G>::Result const &
    operator ()() const
    {
	return get(_t);
    }

    void operator ()(typename FunctionTraits<S>::template Arg<0>::Type arg)
    {
	set(_t, arg);
    }
};

#define PROP_INIT(type, name, start, get, set) Property<type, decltype(get), decltype(set)> name(start, get, set);
#define PROP(type, name, get, set) Property<type, decltype(get), decltype(set)> name(get, set);
