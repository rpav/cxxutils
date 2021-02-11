#pragma once

#include <rpav/str.hpp>
#include <simpleunits/simpleunits.h>

namespace rpav {

template<typename T>
struct unit_name_t {};

template<typename T>
constexpr auto unit_name = unit_name_t<T>();

template<typename T>
struct str_adl<unit_name_t<T>> {
    static void toStream(std::ostream& os, const unit_name_t<T>&) { stream(os, T::unit_name()); }
};

template<typename A, typename B>
struct str_adl<unit_name_t<simpleunits::u_div<A, B>>> {
    static void toStream(std::ostream& os, const unit_name_t<simpleunits::u_div<A, B>>&)
    {
        stream(os, unit_name<A>, "/", unit_name<B>);
    }
};

template<typename U, int K>
struct str_adl<unit_name_t<simpleunits::u_exp<U, K>>> {
    static void toStream(std::ostream& os, const unit_name_t<simpleunits::u_exp<U, K>>&)
    {
        stream(os, unit_name<U>, "^", K);
    }
};

template<typename T, typename U>
struct str_adl<simpleunits::uvalue<T, U>> {
    static void toStream(std::ostream& os, const simpleunits::uvalue<T, U>& v)
    {
        stream(os, *v, "_", unit_name<U>);
    }
};

} // namespace rpav
