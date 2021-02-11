#pragma once

#include <algorithm>
#include <cstddef>
#include <rpav/typeutil.hpp>

namespace rpav {

template<typename T>
class dynarray {
    T*     _data{nullptr};
    size_t _size{0};

    template<typename R>
    class _iterator {
        R* _r{nullptr};

    public:
        using difference_type   = ptrdiff_t;
        using value_type        = typename maybe_const<R, T>::type;
        using reference         = value_type&;
        using pointer           = value_type*;
        using iterator_category = std::random_access_iterator_tag;

        _iterator(R* r) : _r{r} {}
        _iterator(_iterator& i) : _r(i._r) {}
        _iterator(const _iterator& i) : _r(i._r) {}

        bool       operator==(const _iterator& i) const { return _r == i._r; }
        bool       operator!=(const _iterator& i) const { return _r != i._r; }
        _iterator& operator++()
        {
            ++_r;
            return *this;
        }
        _iterator& operator--()
        {
            --_r;
            return *this;
        }

        value_type& operator*() { return *_r; }

        _iterator operator+(difference_type n) const { return {_r + n}; }
        _iterator operator-(difference_type n) const { return {_r - n}; }

        difference_type operator-(const _iterator& i) {
            return _r - i._r;
        }

        _iterator& operator+=(difference_type n)
        {
            _r += n;
            return *this;
        }
        _iterator& operator-=(difference_type n)
        {
            _r -= n;
            return *this;
        }

        reference operator[](difference_type n) { return *(_r + n); }

        bool operator<(const _iterator& i) const { return _r < i._r; }
        bool operator<=(const _iterator& i) const { return _r <= i._r; }
        bool operator>(const _iterator& i) const { return _r > i._r; }
        bool operator>=(const _iterator& i) const { return _r >= i._r; }
    };

public:
    dynarray(size_t sz) : _data{new T[sz]}, _size{sz} {}

    template<typename... Ts>
    dynarray(Ts&&... ts) : _data{new T[sizeof...(Ts)]{ts...}}, _size{sizeof...(Ts)}
    {}

    dynarray(const dynarray& a) : dynarray(a.size())
    {
        std::copy(a._data, a._data + a.size(), _data);
    }

    dynarray(dynarray& a) : dynarray(std::as_const(a)) {}

    dynarray(dynarray&& a) noexcept
    {
        _data   = a._data;
        _size   = a._size;
        a._data = nullptr;
        a._size = 0;
    }

    ~dynarray()
    {
        if(_data) delete[] _data;
    }

    T&       operator[](size_t n) { return _data[n]; }
    const T& operator[](size_t n) const { return _data[n]; }

    size_t size() const { return _size; }

    T*       data() { return _data; }
    const T* data() const { return _data; }

    T& front() { return _data[0]; }
    T& back() { return _data[size() - 1]; }

    const T& front() const { return _data[0]; }
    const T& back() const { return _data[size() - 1]; }

    // Iterators
    using iterator       = _iterator<T>;
    using const_iterator = _iterator<const T>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() { return iterator(_data); }
    iterator end() { return iterator(_data + size()); }

    const_iterator begin() const { return const_iterator(_data); }
    const_iterator end() const { return const_iterator(_data + size()); }

    reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }
};

} // namespace rpav
