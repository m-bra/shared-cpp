
#include <tuple>

template <typename T, typename... R>
union Union
{
    T _value;
    Union<R...> _next;

    template <unsigned int i>
    using Member = std::tuple_element_t<i, std::tuple<T, R...>>;

    template <unsigned int i>
    Member<i> const &get();

    template <unsigned int i>
    void set(Member<i> const &);
};

template <typename T>
union Union<T>
{   
    T _value;

    template <unsigned int i>
    using Member = std::tuple_element_t<i, std::tuple<T>>;
};

template <unsigned int i, typename... T>
typename Union<T...>::template Member<i> const &
_union_get(Union<T...> const &u)
{
    return _union_get<i - 1>(u._next);
}

template <typename... T>
typename Union<T...>::template Member<0> const &
_union_get<0, T...>(Union<T...> const &u)
{
    return u._value;
}

template <unsigned int i, typename... T>
void _union_set(Union<T...> &u, typename Union<T...>::template Member<i> const &v)
{
    _union_set<i - 1>(u._next, v);
}

template <typename... T>
void _union_set(Union<T...> const &u, typename Union<T...>::template Member<0> const &v)
{
    u.value = v;
}

template <typename... T>
struct TaggedUnion
{
    Union<T...> _union;
    unsigned int _tag;

    template <unsigned int i>
    typename Union<T...>::template Member<i> const &
    get()
    {
	
    }
};
