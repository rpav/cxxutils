#pragma once

#include <rpav/str.hpp>
#include <rpav/bitset.hpp>

namespace rpav {

template<typename T>
struct str_adl<radix_<2, T>> {
    static void toStream(std::ostream& os, const radix_<2, T>& v)
    {
        using utype           = std::make_unsigned_t<T>;
        constexpr auto t_bits = std::numeric_limits<utype>::digits;

        const auto uval{v};

        static const char* table[] = {"0000", "0001", "0010", "0011", "0100", "0101",
                                      "0110", "0111", "1000", "1001", "1010", "1011",
                                      "1100", "1101", "1110", "1111"};

        if(v.showBase) stream(os, "0b");

        for(int j = t_bits / 4 - 1; j >= 0; --j) {
            utype   mask = utype{0x0F} << (j * 4);
            uint8_t val  = (uval & mask) >> (j * 4);
            stream(os, table[val]);
        }
    }
};

template<size_t N, typename T>
struct str_adl<radix_<N, T>> {
    static void toStream(std::ostream& os, const radix_<N, uint8_t>& v)
    {
        auto f = os.flags();
        stream(
            os, v.showBase ? std::showbase : std::noshowbase, std::setbase(N), (unsigned)v.v);
        os.flags(f);
    }

    static void toStream(std::ostream& os, const radix_<N, int8_t>& v)
    {
        auto f = os.flags();
        stream(os, v.showBase ? std::showbase : std::noshowbase, std::setbase(N), (int)v.v);
        os.flags(f);
    }

    template<typename S>
    static void toStream(std::ostream& os, const radix_<N, S>& v)
    {
        auto f = os.flags();
        stream(os, v.showBase ? std::showbase : std::noshowbase, std::setbase(N), v.v);
        os.flags(f);
    }
};

template<typename T, size_t N>
void toStream(std::ostream& os, const basic_bitset<T, N>& bs)
{
    stream(os, "<bitset", bs.t_bits, ":", bs.size(), ":");
    for(auto i = bs.data().size(); i > 0; --i) {
        auto entry = bs.data()[i - 1];
        stream(os, radix<2>(entry));
    }
    stream(os, ">");
}

} // namespace rpav
