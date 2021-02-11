#pragma once

#include <rpav/iterator/adaptor.hpp>

namespace rpav::iter {

template<typename T>
class reverse_adaptor : public iterator_adaptor<T, void, typename reverse_iterator_extractor<T>::type> {
public:
    using adaptor = iterator_adaptor<T, void, typename reverse_iterator_extractor<T>::type>;
    using iterator = typename adaptor::iterator;

    using adaptor::adaptor;

    iterator begin() { return iterator(std::rbegin(this->container)); }
    iterator end() { return iterator(std::rend(this->container)); }

    iterator begin() const { return iterator(std::rbegin(this->container)); }
    iterator end() const { return iterator(std::rend(this->container)); }
};

template<typename T>
reverse_adaptor<T> reversing(T &v) {
    return reverse_adaptor<T>(v);
}

template<typename T>
reverse_adaptor<T&&> reversing(T &&v) {
    return reverse_adaptor<T&&>(std::forward<T>(v));
}


}