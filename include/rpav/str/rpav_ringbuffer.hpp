#pragma once

#include <rpav/ringbuffer.hpp>
#include <rpav/str.hpp>

namespace rpav {
template<typename T>
inline void toStream(std::ostream& os, const ringbuffer <T>& r) {
    stream(os, "ring[");


    for(auto[last, j] : iter::zip(iter::is_last(r), r)) {
        stream(os, j);

        if(!last) stream(os, ", ");
    }

    stream(os, "]");
}
}
