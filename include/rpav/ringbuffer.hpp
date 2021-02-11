#pragma once

#include <vector>
#include <rpav/iterator.hpp>
#include <rpav/dynarray.hpp>
#include <rpav/log.hpp>

namespace rpav {

template<typename T>
class ringbuffer {
    dynarray<T> _data;
    size_t      _fill{0};
    size_t      _next{0};

    template<typename R>
    class _iterator {
        R& _r;
        size_t _idx;

    public:
        using difference_type = ptrdiff_t;
        using value_type = typename maybe_const<R, T>::type;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        _iterator(R& r, size_t i) : _r(r), _idx(i) { }
        _iterator(_iterator& i) : _r(i._r), _idx(i._idx) { }
        _iterator(const _iterator& i) : _r(i._r), _idx(i._idx) { }

        bool operator==(const _iterator& i) const { return _idx == i._idx; }
        bool operator!=(const _iterator& i) const { return _idx != i._idx; }
        _iterator& operator++() {
            ++_idx;
            return *this;
        }
        _iterator& operator--() {
            --_idx;
            return *this;
        }

        value_type& operator*() { return _r[_idx]; }
    };

public:

    ringbuffer(size_t max) : _data(max) { }

    template<typename...Ts>
    T& emplace_back(Ts&& ...ts) {
        if(_fill < capacity()) ++_fill;

        auto &ref = _data[_next] = T(std::forward<Ts>(ts)...);

        ++_next;
        if(_next >= capacity()) { _next = 0; }

        return ref;
    }

    T& push_back(const T& t) {
        return emplace_back(t);
    }

    T& push_back(T&& t) {
        return emplace_back(std::move(t));
    }

    const T& operator[](size_t n) const {
        auto offs = capacity() - fill();
        return _data[(n + _next + offs) % capacity()];
    }

    T& operator[](size_t n) {
        return const_cast<T&>(std::as_const(*this).operator[](n));
    }

    T& front() { return (*this)[0]; }
    const T& front() const { return (*this)[0]; }

    T& back() { return (*this)[fill()-1]; }
    const T& back() const { return (*this)[fill()-1]; }

    void pop_back() {
        --_fill;

        if(_next == 0) {
            _next = capacity() - 1;
        } else {
            --_next;
        }
    }

    void clear() {
        _fill = _next = 0;
    }

    size_t size() const { return _fill; }
    size_t fill() const { return _fill; }
    size_t capacity() const { return _data.size(); }

    // Iterators
    using iterator = _iterator<ringbuffer<T>>;
    using const_iterator = _iterator<const ringbuffer<T>>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, size()); }

    const_iterator begin() const { return const_iterator(*this, 0); }
    const_iterator end() const { return const_iterator(*this, size()); }

    reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }
};

}