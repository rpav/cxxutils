#pragma once

#include <rpav/str.hpp>
#include <rpav/bitset.hpp>

namespace rpav {

inline void toStream(std::ostream& os, const variable_bitset& bs)
{
    stream(os, "<", bs.size(), ":");
    for(auto i = bs.wordSize(); i > 0; --i) {
        auto word = bs.getWord(i - 1);

        for(auto j = bs.bit_size; j > 0; --j) {
            auto bit = word & (variable_bitset::word_type(1) << (j - 1));
            stream(os, bit ? "1" : "0");
        }
    }
    stream(os, ">");
}

}
