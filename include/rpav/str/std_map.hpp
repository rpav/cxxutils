#pragma once

#include <iomanip>
#include <map>

#include <rpav/iterator.hpp>
#include <rpav/str.hpp>
#include <rpav/str/std_utility.hpp>

/* Printing for std::vector */

namespace rpav {

template<typename K, typename V, typename...Ts>
struct str_adl<std::map<K, V, Ts...>> {
    static void toStream(std::ostream& s, const std::map<K, V, Ts...>& m)
    {
        using namespace rpav::iter;

        stream(s, "{");

        for(auto [last, j] : zip(is_last(m), m)) {
            stream(s, j);
            if(!last) stream(s, ", ");
        }

        stream(s, "}");
    }
};

} // namespace rpav
