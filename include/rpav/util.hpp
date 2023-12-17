#pragma once

#include <cstddef>
#include <cstring>
#include <functional>

namespace rpav {

template<typename T>
T* as_ptr(T& t)
{
    return &t;
}

template<typename T>
T* as_ptr(T* t)
{
    return t;
}

template<typename T, size_t sz>
size_t elements_of(const T (&)[sz])
{
    return sz;
}

template<typename R, typename C, typename... Ts>
auto mbind(R (C::*mptr)(Ts...), C& that) -> std::function<R(Ts...)>
{
    return [mptr, &that](Ts... ts) { return std::invoke(mptr, that, ts...); };
}

template<typename S, typename T>
void memcpy(S&& dest, T&& src)
{
    using dS = std::remove_pointer_t<std::remove_reference_t<S>>;
    using dT = std::remove_pointer_t<std::remove_reference_t<T>>;

    static_assert(sizeof(dS) == sizeof(dT) && "Copying between differently-sized types");
    std::memcpy((void*)as_ptr(dest), (void*)as_ptr(src), sizeof(dS));
}

template<typename T>
void memset(T&& t, unsigned char v)
{
    using dT = std::remove_pointer_t<std::remove_reference_t<T>>;
    std::memset((void*)as_ptr(t), v, sizeof(dT));
}

template<typename T>
void memzero(T&& t)
{
    memset(t, 0);
}

} // namespace rpav
