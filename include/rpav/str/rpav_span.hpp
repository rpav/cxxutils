#pragma once

#include <iomanip>
#include <rpav/iterator.hpp>
#include <rpav/span.hpp>
#include <rpav/str.hpp>

namespace rpav {

template<typename T>
inline void toStream(std::ostream &s, const span<T> &v) {
    stream(s, "[");

    for(auto &&[i, val] : iter::zip(iter::from<size_t>(0), v)) {
        stream(s, val);
        if(i < v.size()-1)
            stream(s, ", ");
    }

    stream(s, "]");
}

}
