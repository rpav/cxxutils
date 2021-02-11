#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <rpav/iterator.hpp>

namespace rpav {

template<typename T>
class span {
    T* _begin{nullptr};
    T* _end{nullptr};

public:
    using value_type = T;

    template<typename S>
    span(S &v, size_t start, size_t length) {
        if(start + length > v.size())
            throw std::range_error{"span exceeds size of source"};

        _begin = &v[start];
        _end = &v[start + length];
    }

    template<typename S, typename Iter = typename S::iterator>
    span(S &v) {
        if(v.size() == 0) return;

        _begin = &v.front();
        _end = &v.back() + 1;
    }

    span(T *s, T *e) {
        if(s > e) throw std::range_error{"end must be after start"};

        _begin = s;
        _end = e;
    }

    span(T* v, size_t len) {
        _begin = v;
        _end = v + len;
    }

    template<typename S, size_t sz>
    span(S (&v)[sz]) : span(&v[0], &v[sz]) { }

    T& operator[] (size_t index) { return _begin[index]; }
    const T& operator[] (size_t index) const { return _begin[index]; }

    const T& at(size_t index) const {
        if(index < size()) return _begin[index];
        throw std::range_error{"Index out of range"};
    }

    T& at(size_t index) {
        return const_cast<T&>(std::as_const(*this).at(index));
    }

    bool empty() const { return _begin == _end; }

    span subspan(size_t start, size_t len) const {
        if(start + len > size())
            throw std::range_error{"subspan exceeds dimensions"};

        return span(&_begin[start], &_begin[start+len]);
    }

    /// Return a subspan from `start` to the end
    /// \param start
    /// \return subspan
    span from(size_t start) const {
        return subspan(start, size()-start);
    }

    /// Return a subspan in the range [0,len]
    /// \param len
    /// \return
    span first(size_t len) const {
        return subspan(0, len);
    }

    span last(size_t len) const {
        return subspan(size()-len, len);
    }

    size_t size() const { return _end - _begin; }

    T& front() { return _begin[0]; }
    const T& front() const { return _begin[0]; }

    T& back() { return _begin[size()-1]; }
    const T& back() const { return _begin[size()-1]; }


public:

    template<typename S=T>
    class _iterator : public iterator_def<
        std::bidirectional_iterator_tag,
        S,
        std::ptrdiff_t,
        S*,
        S&
    >
    {

    private:
        S *_v;

    public:
        _iterator(value_type *v) : _v{v} { }
        _iterator(_iterator &i) : _v{i._v} { }
        _iterator(const _iterator &i) : _v{i._v} { }

        bool operator==(const _iterator &i) const {
            return _v == i._v;
        }

        bool operator!= (const _iterator &i) const {
            return !(*this == i);
        }

        _iterator& operator++() { ++_v; return *this; }
        _iterator& operator--() { --_v; return *this; }

        S& operator*() { return *_v; }
        const S& operator*() const { return *_v; }

        operator S*() { return _v; }

        S* get() { return _v; }
        const S* get() const { return _v; }
    };

    using iterator = _iterator<T>;
    using const_iterator = _iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() { return iterator{_begin}; }
    iterator end() { return iterator{_end}; }

    const_iterator begin() const { return const_iterator{_begin}; }
    const_iterator end() const { return const_iterator{_end}; }

    reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }

    span(iterator &first, iterator &last)
        : _begin{first}
        , _end{last}
    { }
};

template<class Cont, typename...Rest>
span(Cont&, Rest&&...) -> span<typename Cont::value_type>;

template<class Cont, typename...Rest>
span(const Cont&, Rest&&...) -> span<const typename Cont::value_type>;

template<typename S, size_t sz>
span(const S (&v)[sz]) -> span<S>;

}
