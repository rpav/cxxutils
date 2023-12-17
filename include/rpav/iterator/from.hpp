#pragma once

#include <limits>

namespace rpav::iter {

template<typename T>
struct from_bound {
    T _v;
    from_bound(T v) : _v(v) { }
};

template<typename T>
struct from_iterator : public iterator_def<std::forward_iterator_tag, T, T, T*, T&> {
    from_iterator(T v, T i) : _v(v), _i(i) { }

    T& operator*() { return _v; }
    T* operator->() { return &_v; }

    from_iterator& operator++() { _v += _i; return *this; }

    bool operator==(const from_iterator &i) const { return _v == i._v; }
    bool operator!=(const from_iterator &i) const { return _v != i._v; }

    bool operator==(const from_bound<T> &b) const {
        if(_i < 0) return _v <= b._v;

        return _v >= b._v;
    }

    bool operator!=(const from_bound<T> &b) const {
        if(_i < 0) return _v > b._v;

        return _v < b._v;
    }

private:
    T _v;
    T _i;
};


/**
 * Iterator from `v` by `i` to the maximum possible for T.
 */
template<typename T>
struct from {
    using iterator = from_iterator<T>;

    /**
     * Iterate from `v` by `i`.
     * @param v Starting value
     * @param i Increment (default 1)
     */
    from(T v, T i = T(1)) : _v(v), _i(i) { }

    iterator begin() const { return iterator(_v, _i); }
    from_bound<T> end() const { return from_bound<T>(std::numeric_limits<T>::max()); }

private:
    T _v;
    T _i;
};


template<typename T>
struct from_to {
    using iterator = from_iterator<T>;

    from_to(T s, T e, T i = T(1))
        : _from(s), _to(e), _i(i)
    { }

    iterator begin() const { return iterator(_from, _i); }
    from_bound<T> end() const { return from_bound<T>(_to); }

private:
    T _from;
    T _to;
    T _i;
};

}
