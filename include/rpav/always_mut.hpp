#pragma once

namespace rpav {

template<typename T>
struct always_mut {
    operator T &() const { return v; }
    T *operator->() const { return &v; }

    template<typename...Rest>
    always_mut(Rest &&...r) : v(r...) { }
    always_mut() = default;

private:
    mutable T v;
};

} // namespace rpav