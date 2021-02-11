#pragma once

#include <limits>
#include <functional>

namespace rpav {

// This is more or less overkill exclusively for providing default
// initialization for basic types.
template<typename T>
class value {
    T _val{};

public:
    using type = T;

    constexpr value() = default;

    template<typename U>
    inline constexpr value(U v) noexcept : _val{static_cast<T>(v)}
    {}

    inline           operator T&() noexcept { return _val; }
    inline constexpr operator const T&() const noexcept { return _val; }
};

template<typename T, typename T::type V>
struct value_lit {
    using type = T;
    using raw_type = typename T::type;
    static constexpr T value = V;
};

} // namespace rpav

namespace std {

template<typename T>
struct hash<rpav::value<T>> : public hash<T> {};

template<typename T>
struct numeric_limits<rpav::value<T>> : public numeric_limits<T> {};

} // namespace std
