#pragma once

#include <algorithm>
#include <functional>
#include <tuple>

namespace rpav {

template<typename F, typename T>
decltype(auto) static_fold(F&& f, T&& a, T&& b) {
    return f(a,b);
}

template<typename F, typename T, typename...Ts>
decltype(auto) static_fold(F&& f, T&& a, T&& b, Ts&&...ts)
{
    return static_fold(f, f(a,b), std::forward<Ts>(ts)...);
}

template<typename T>
std::tuple<T, T, bool> swap_if(T &&a, T &&b, bool test) {
    if(test) return {b, a, true};
    return {a, b, false};
}

template<typename T>
std::tuple<T, T, bool> swap_if(T &&a, T &&b, const std::function<bool()>& test) {
    if(test()) return {b, a, true};
    return {a, b, false};
}

template<typename T, typename Pred, typename I = typename T::iterator>
inline void remove_if(T& c, I start, I end, Pred p) {
    auto e = std::remove_if(start, end, p);
    c.erase(e, end);
}

}
