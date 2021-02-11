#pragma once

#include <rpav/iterator/adaptor.hpp>

namespace rpav::iter {

template<typename T>
struct is_last_iterator {
    is_last_iterator(T s, T e) : _it(s), _last(e) { }

    is_last_iterator& operator++() { ++_it; return *this; }

    bool operator==(const is_last_iterator &it) const {
        return _it == it._it;
    }
    bool operator!=(const is_last_iterator &it) const {
        return _it != it._it;
    }

    bool operator*() const {
        auto it = _it;
        ++it;
        return it == _last;
    }

private:
    T _it;
    T _last;
};

template<typename T>
struct is_last {
    using inner_iterator = typename iterator_extractor<const T>::type;
    using iterator = is_last_iterator<inner_iterator>;

    is_last(const T &v) : _cont(v) { }

    iterator begin() const {
        auto e = std::end(_cont);
        return iterator(std::begin(_cont), e);
    }
    iterator end() const {
        auto e = std::end(_cont);
        return iterator(std::end(_cont), e);
    }

private:
    const T& _cont;
};

} // namespace rpav::iter
