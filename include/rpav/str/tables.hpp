#pragma once

#include <rpav/str.hpp>
#include <tables/dump.hpp>

namespace rpav {

template<size_t N, typename... Ts>
struct str_adl<tables::basic_iterator<N, Ts...>> {
    using iterator_type = tables::basic_iterator<N,Ts...>;

    static void toStream(std::ostream& os, const iterator_type& row)
    {
        stream(os, "<iter:", row.index(), ":", *row, ">");
    }
};

} // namespace tables
