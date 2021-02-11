#pragma once

#include <rpav/str.hpp>

namespace rpav {

template<typename A>
struct str_adl<const std::function<A>&> {
    static void toStream(std::ostream &s, const std::function<A>&) {
        rpav::stream(s, "std::function<...>");
    }
};

}