#pragma once

#include <rpav/str.hpp>
#include <rpav/iterator.hpp>
#include <rpav/dynarray.hpp>

namespace rpav {

template<typename T>
inline void toStream(std::ostream& os, const rpav::dynarray<T> &a) {
    stream(os, "[");

    for(auto [last, v] : iter::zip(iter::is_last(a), a)) {
        stream(os, v);

        if(!last) stream(os, ", ");
    }

    stream(os, "]");
}

}
