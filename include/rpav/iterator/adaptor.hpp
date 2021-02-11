#pragma once

#include <rpav/typeutil.hpp>
#include <rpav/iterator.hpp>

namespace rpav::iter {

template<
    typename T,
    typename Sub = void,
    typename Iter = typename iterator_extractor<T>::type,
    typename Cat = typename std::iterator_traits<Iter>::iterator_category,
    typename Val = typename std::iterator_traits<Iter>::value_type,
    typename Diff = typename std::iterator_traits<Iter>::difference_type,
    typename Ptr = typename std::iterator_traits<Iter>::pointer,
    typename Ref = typename std::iterator_traits<Iter>::reference

>
class iterator_wrapper {
public:
    using crtp = typename or_type<Sub, iterator_wrapper>::type;
    using inner_iterator = Iter;
    using iterator_category = Cat;
    using value_type = Val;
    using difference_type = Diff;
    using pointer = Ptr;
    using reference = Ref;

    iterator_wrapper(const inner_iterator &it) : inner(it) { }

    reference operator*() { return *inner; }
    pointer operator->() { return inner; }

    crtp &operator++() {
        ++inner;
        return static_cast<crtp&>(*this);
    }
    crtp operator++(int) {
        crtp tmp(static_cast<crtp&>(*this));
        ++*this;
        return tmp;
    }

    crtp& operator--() {
        --inner;
        return static_cast<crtp&>(*this);
    }
    crtp operator--(int) {
        crtp tmp(*this);
        --*this;
        return tmp;
    }

    bool operator==(const iterator_wrapper &it) const { return inner == it.inner; }
    bool operator!=(const iterator_wrapper &it) const { return !(*this == it); }

protected:
    inner_iterator inner;
};

template<
    typename T,
    typename SubIter = void,
    typename InnerIter = typename iterator_extractor<T>::type,
    typename Local = typename local_inst<T>::type
>
class iterator_adaptor {
protected:
    using wrapper = iterator_wrapper<T, SubIter, InnerIter>;

public:
    using iterator = wrapper;

    iterator_adaptor(T &t) : container(t) { }
    iterator_adaptor(T &&t) : container(std::forward<T>(t)) { }

protected:
    Local container;
};

}
