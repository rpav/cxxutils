#pragma once

#include <tuple>
#include <optional>
#include <type_traits>

namespace rpav {

template<typename T>
using opt = std::optional<T>;

template<typename T>
T copy(const T &v) { return v; }

template<typename T, typename V>
inline T cast(V&& v) {
    return static_cast<T>(std::forward<V>(v));
}

template<typename K, typename V>
auto p_(K&& k, V&& v) {
    return std::pair(k,v);
}

template<typename...Ts>
auto t_(Ts&&...ts) {
    return std::tuple{std::forward<Ts>(ts)...};
}

}
