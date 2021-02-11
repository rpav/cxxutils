#pragma once

#include <vector>
#include <rpav/typeutil.hpp>

namespace rpav {

template<typename T>
class DynamicBind;

template<typename T>
class DynamicScope {
    std::vector<T> _stack;

protected:
    template<typename...Rest>
    void emplace(Rest&&...r) {
        _stack.emplace_back(std::forward<Rest>(r)...);
    }

    void pop() {
        _stack.pop_back();
    }

    friend class DynamicBind<T>;

public:
    using ptr_return = typename rpav::ref_or_ptr<T>::type;

    DynamicScope() = default;

    template<typename...Rest>
    DynamicScope(Rest&&...rest) {
        emplace(std::forward<Rest>(rest)...);
    }

    ptr_return operator->() { return _stack.back(); }
    const ptr_return operator->() const { return _stack.back(); }

    T& operator*() { return _stack.back(); }
    const T& operator*() const { return _stack.back(); }

    operator bool() const { return _stack.size() > 0; }

    T& operator=(const T &v) {
        if(_stack.size() == 0) {
            _stack.emplace_back(v);
        } else {
            _stack.back() = v;
        }

        return _stack.back();
    }
};

/**
 * Usage:
 *   DynamicBind _bind(VARIABLE, value);
 *
 * @tparam T
 */
template<typename T>
class DynamicBind {
    DynamicScope<T> *_v;
public:
    template<typename...Rest>
    DynamicBind(DynamicScope<T> &v, Rest&&...rest) : _v(&v) {
        _v->emplace(std::forward<Rest>(rest)...);
    }

    DynamicBind(DynamicBind&& b) : _v(b._v) {
        b._v = nullptr;
    }

    ~DynamicBind() {
        if(_v) _v->pop();
    }
};

}

#ifndef LET
#define LET(x,...) DynamicBind BIND_ ## x(x, __VA_ARGS__)
#else
#warning "LET already defined, not redefining"
#endif