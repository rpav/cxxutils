#pragma once

#include <rpav/iterator/adaptor.hpp>

namespace rpav::iter {

template<typename T>
struct with_iterator_iterator : public iterator_wrapper<T, with_iterator_iterator<T>>  {
    using wrapper = iterator_wrapper<T, with_iterator_iterator<T>>;

    using wrapper::wrapper;

    typename wrapper::inner_iterator& operator*() {
        return this->inner;
    }

    typename wrapper::inner_iterator* operator->() {
        return &this->inner;
    }
};

/**
 * Iterate, with an iterator.  Useful for range-based for on things
 * whose iterator holds important data other than `*iter`.
 */
template<typename T>
class with_iterator_adaptor : public iterator_adaptor<T> {
    using adaptor = iterator_adaptor<T>;
    using wrapper = typename adaptor::iterator;

public:
    using iterator = with_iterator_iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    using adaptor::adaptor;

    iterator begin() { return iterator(this->container.begin()); }
    iterator end() { return iterator(this->container.end()); }

    reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() { return std::make_reverse_iterator(begin()); }
};

template<typename T>
with_iterator_adaptor<T> with_iterator(T &v) {
    return with_iterator_adaptor<T>(v);
}

template<typename T>
with_iterator_adaptor<T&&> with_iterator(T &&v) {
    return with_iterator_adaptor<T&&>(std::forward<T>(v));
}

}