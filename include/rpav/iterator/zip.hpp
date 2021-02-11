#pragma once

#include <tuple>
#include <rpav/typeutil.hpp>
#include <functional>

namespace rpav::iter {

template<typename...Is>
struct zipper_iterator {
    zipper_iterator(Is...ts)
        : _its{std::forward<Is>(ts)...}
    { }

    zipper_iterator& operator++() {
        std::apply(
            [](auto&...it) { (++it, ...); }
            , _its
        );

        return *this;
    }

    auto operator*() {
        return std::apply(
            [](auto&&...it) { return std::make_tuple(maybe_ref(*it)...); }
            , _its
        );
    }

    template<typename Zi>
    bool operator!= (const Zi& zi) const {
        bool eq = false;
        any_eq<>(zi, eq);
        return !eq;
    }

    template<typename Zi>
    bool operator== (const Zi& zi) const {
        bool eq = false;
        any_eq<>(zi, eq);
        return eq;
    }

private:
    std::tuple<Is...> _its;

    template<typename...Ts>
    friend struct zipper_iterator;

    template<typename Zi, size_t N = 0>
    inline void any_eq(const Zi &zi, bool &eq) const {
        if(eq) return;

        if constexpr(N < sizeof...(Is)) {
            auto &i0 = std::get<N>(_its);
            auto &i1 = std::get<N>(zi._its);

            eq = (i0 == i1);
            any_eq<Zi, N+1>(zi, eq);
        }
    }
};


template<typename...Cs>
class zipper {
public:
    template<typename...Ts>
    zipper(Ts&&...ts) : _conts{std::forward<Ts>(ts)...} { }

    auto begin() {
        return
            std::apply(
                [](auto&...c) { return zipper_iterator(std::begin(c)...); }
                , _conts
            );
    }
    auto end() {
        return std::apply(
            [](auto&...c) { return zipper_iterator(std::end(c)...); }
            , _conts
        );
    }
private:
    std::tuple<Cs...> _conts;
};

template<typename...Ts>
zipper<Ts...> zip(Ts&&...ts) {
    return zipper<Ts...>(std::forward<Ts>(ts)...);
}

} // namespace rpav::iter