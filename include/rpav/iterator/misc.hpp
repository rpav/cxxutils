#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace rpav::iter {

/**
 * Iterate across a pair, i.e. from `p.first` to `p.second`.
 *
 * @tparam T
 */
template<typename T>
class across_pair_adaptor {
public:
    using iterator = T;
    using pair_type = std::pair<T,T>;

private:
    pair_type &_pair;

public:
    across_pair_adaptor(pair_type &p) : _pair(p) { }

    iterator begin() { return _pair.first; }
    iterator end() { return _pair.second; }
};

template<typename T, size_t N>
struct eachof {
    using type = std::decay_t<T>;

    std::array<type*, N> _vals;

    template<typename... Ss>
    eachof(Ss&... vs) : _vals{&vs...}
    {}

    struct iterator {
        type** v{};

        iterator(type** v) : v(v) {}

        type&     operator*() { return **v; }
        iterator& operator++()
        {
            ++v;
            return *this;
        }
        bool operator==(const iterator& b) const { return v == b.v; }
        bool operator!=(const iterator& b) const { return !(*this == b); }
    };

    iterator begin() { return iterator(&_vals[0]); }
    iterator end() { return iterator(&_vals[N]); }
};

template<typename T, typename...Ts>
eachof(T, Ts...) -> eachof<T, sizeof...(Ts)+1>;

}
