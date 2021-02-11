#pragma once

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


}