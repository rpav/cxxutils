#pragma once

#include <map>
#include <iterator>

#include <rpav/iterator.hpp>
#include <rpav/ptr.hpp>

namespace rpav {

template<typename M, typename T>
inline remove_ref_t<typename M::mapped_type>* findval(M& m, T&& k)
{
    auto&& it = m.find(k);

    if(it != m.end()) return &it->second;

    return nullptr;
}

template<typename M, typename T>
inline remove_ref_t<const typename M::mapped_type>* findval(const M& m, T&& k)
{
    auto&& it = m.find(k);

    if(it != m.end()) return &it->second;

    return nullptr;
}

template<typename C, typename T, typename F>
inline void if_find(C& c, T&& k, F f)
{
    auto&& it = c.find(k);

    if(it != c.end()) f(it->second);
}

template<typename C, typename T, typename F>
inline void if_find(const C& c, T&& k, F f)
{
    auto&& it = c.find(k);

    if(it != c.end()) f(it->second);
}

template<typename C, typename T = typename C::value_type>
void ensure(C& c, typename C::size_type sz)
{
    c.resize(c.size() > sz ? c.size() : sz);
}

template<typename T, typename...Ts>
std::array<T, sizeof...(Ts)+1> make_array(T&& v, Ts&&...vs) {
    return {v, vs...};
}


} // namespace rpav
