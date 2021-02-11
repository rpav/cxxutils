#pragma once

#include <utility>

namespace rpav {

template<typename T>
class modify_track {
    bool _modified{true};
    T _value;

public:
    template<typename...Ts>
    explicit modify_track(Ts&&...ts) : _value(std::forward<Ts>(ts)...) { }

    const T& operator*() const { return _value; }
    operator const T&() const { return _value; }

    const T* operator->() const { return &_value; }

    T& get() { _modified = true; return _value; }

    bool isModified() const { return _modified; }
    void reset() { _modified = false; }
};

}