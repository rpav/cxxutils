#pragma once

#include <iomanip>
#include <list>
#include "rpav/iterator.hpp"
#include "rpav/str.hpp"

/* Printing for std::list */

namespace rpav {

template<typename T>
struct str_adl<std::list<T>> {
    static void toStream(std::ostream &s, const std::list<T> &v) {
        using namespace rpav::iter;

        stream(s, "(");

        for(auto [last, j] : zip(is_last(v), v)) {
            stream(s, j);
            if(!last) stream(s, ", ");
        }

        stream(s, ")");
    }
};

// FIXME: This really needs unified down to an element-specific handler
template<>
struct str_adl<std::list<std::string>> {
    static void toStream(std::ostream &s, const std::list<std::string> &v) {
        using namespace rpav::iter;

        stream(s, "(");

        for(auto [last, j] : zip(is_last(v), v)) {
            stream(s, std::quoted(j));
            if(!last) stream(s, ", ");
        }

        stream(s, ")");
    }
};


}
