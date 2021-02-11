#pragma once

#include <iomanip>
#include <limits>

#include <rpav/typeutil.hpp>

#if defined(_MSC_VER)
#    include <intrin.h>
#    pragma intrinsic(_BitScanReverse)
#endif

namespace rpav {
#if (defined(__GNUC__) || defined(__clang__))
template<typename T>
constexpr inline size_t clz(T v)
{
    constexpr auto bits   = std::numeric_limits<T>::digits;
    constexpr auto ubits  = std::numeric_limits<unsigned>::digits;
    constexpr auto lbits  = std::numeric_limits<unsigned long>::digits;
    constexpr auto llbits = std::numeric_limits<unsigned long long>::digits;

    static_assert(bits <= llbits);

    if constexpr(bits > lbits && bits <= llbits) {
        return v ? __builtin_clzll(v) - (llbits - bits) : bits;
    } else if(bits > ubits && bits <= lbits) {
        return v ? __builtin_clzl(v) - (lbits - bits) : bits;
    } else {
        return v ? __builtin_clz(v) - (ubits - bits) : bits;
    }
}

template<typename T>
constexpr inline size_t bsr(T v)
{
    constexpr auto bits = std::numeric_limits<T>::digits;
    return bits - clz(v) - 1;
}

template<typename T>
constexpr inline size_t ctz(T v)
{
    constexpr auto bits   = std::numeric_limits<T>::digits;
    constexpr auto ubits  = std::numeric_limits<unsigned>::digits;
    constexpr auto lbits  = std::numeric_limits<unsigned long>::digits;
    constexpr auto llbits = std::numeric_limits<unsigned long long>::digits;

    static_assert(bits <= llbits);

    if constexpr(bits > lbits && bits <= llbits) {
        return v ? __builtin_ctzll(v) : bits;
    } else if(bits > ubits && bits <= lbits) {
        return v ? __builtin_ctzl(v) : bits;
    } else {
        return v ? __builtin_ctz(v) : bits;
    }
}

template<typename T>
constexpr inline size_t popcount(T v)
{
    constexpr auto bits   = std::numeric_limits<T>::digits;
    constexpr auto ubits  = std::numeric_limits<unsigned>::digits;
    constexpr auto lbits  = std::numeric_limits<unsigned long>::digits;
    constexpr auto llbits = std::numeric_limits<unsigned long long>::digits;

    static_assert(bits <= llbits);

    if constexpr(bits > lbits && bits <= llbits) {
        return __builtin_popcountll(v);
    } else if(bits > ubits && bits <= lbits) {
        return __builtin_popcountl(v);
    } else {
        return __builtin_popcount(v);
    }
}

#elif defined(_MSC_VER)
#    warning TEST
template<typename T>
constexpr inline T clz(T v)
{
    unsigned long index{};
    _BitScanReverse(&index, v);
    return (*index ? *index - 1 : std::numeric_limits<T>::digits);
}

template<typename T>
constexpr inline T bsr(T v)
{
    constexpr auto bits = std::numeric_limits<T>::digits;
    return clz(v) + bits - 1;
}

template<typename T>
constexpr inline T ctz(T v)
{
    unsigned long index{};
    _BitScanForward(&index, v);
    return (*index ? *index - 1 : std::numeric_limits<T>::digits);
}
#else

// Probably-not-the-fastest backup implementations

// Returns the *BIT INDEX*, from the LSB = 0 to MSB = 31
constexpr inline size_t bsr32(uint32_t v)
{
    // From wikipedia under CLZ
    size_t r{};
    size_t q{};

    // clang-format off
    r = (v > 0xFFFF) << 4; v >>= r;
    q = (v > 0xFF  ) << 3; v >>= q; r |= q;
    q = (v > 0xF   ) << 2; v >>= q; r |= q;
    q = (v > 0x3   ) << 1; v >>= q; r |= q;
                                    r |= (v >> 1);
    // clang-format on

    return r;
}

template<typename T>
constexpr inline size_t bsr(T v)
{
    using utype         = std::make_unsigned_t<T>;
    constexpr auto bits = std::numeric_limits<utype>::digits;
    static_assert(bits <= 64);

    if constexpr(bits <= 32) {
        return bsr32(v);
    } else {
        const auto a = bsr32((uint32_t)(v >> 32));
        return a ? a + 32 : bsr32(v & 0xFFFFFFFFU);
    }
}

template<typename T>
constexpr inline size_t clz(T v)
{
    using utype         = std::make_unsigned_t<T>;
    constexpr auto bits = std::numeric_limits<utype>::digits;

    return v ? bits - bsr(v) - 1 : bits;
}

constexpr inline size_t ctz32(uint32_t v)
{
    constexpr unsigned char table[32] = {0, 1, 0, 2, 0, 0, 24, 3, 0, 0, 0, 0, 0, 0, 4, 8,
                                         0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0};
    return table[((v & -v) * 0x077CB531) >> 27];
}

template<typename T>
constexpr inline size_t ctz(T v)
{
    constexpr auto bits = std::numeric_limits<T>::digits;

    static_assert(bits <= 64);

    if constexpr(bits <= 32) {
        return v ? ctz32(v) : bits;
    } else {
        const auto v32_0 = static_cast<uint32_t>(v);
        const auto v32_1 = static_cast<uint32_t>(v >> 32);

        if(v) {
            const auto a = ctz(v32_0);
            return (a < 32) ? a : ctz(v32_1) + 32;
        }
        return bits;
    }
}

// From Wikipedia (Hamming_weight), from "Warren Jr., Henry S. (2013)
// [2002]. Hacker's Delight (2 ed.). Addison Wesley - Pearson Education,
// Inc. pp. 81–96. ISBN 978-0-321-84268-8. 0-321-84268-5."

// clang-format off
inline size_t popcount64(uint64_t x)
{
    constexpr uint64_t m1  = 0x5555555555555555; // binary: 0101...
    constexpr uint64_t m2  = 0x3333333333333333; // binary: 00110011..
    constexpr uint64_t m4  = 0x0f0f0f0f0f0f0f0f; // binary:  4 zeros,  4 ones ...
    constexpr uint64_t h01 = 0x0101010101010101; // the sum of 256 to the power of 0,1,2,3...

    x -= (x >> 1) & m1;                   // put count of each 2 bits into those 2 bits
    x  = (x & m2) + ((x >> 2) & m2);      // put count of each 4 bits into those 4 bits
    x  = (x + (x >> 4)) & m4;             // put count of each 8 bits into those 8 bits
    return uint64_t{x * h01} >> 56;       // returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
}
// clang-format on

template<typename T>
inline size_t popcount(T v)
{
    return popcount64(v);
}
#endif

// Make a mask of `n` bits, `offset` from the right, e.g.
// makeMask(4, 2) => 0b00111100 for uint16_t
template<typename T>
constexpr static T bitMaskR(size_t n, size_t offs)
{
    using utype           = typename promote_unsigned<T>::type;
    constexpr auto t_bits = std::numeric_limits<T>::digits;
    constexpr auto u_bits = std::numeric_limits<utype>::digits;

    return static_cast<T>(((~utype{0} >> (t_bits - n)) << offs) >> (u_bits - t_bits));
}

// Make a mask of `n` bits, `offset` from the left, e.g.
// maskMask(2,1) => 0b01100000 for uint16_t
template<typename T>
constexpr static T bitMaskL(size_t n, size_t offs)
{
    using utype           = typename promote_unsigned<T>::type;
    constexpr auto t_bits = std::numeric_limits<T>::digits;


    return static_cast<T>((~utype{0} << (t_bits - n)) >> offs);
}

template<size_t N, typename T>
struct radix_ {
    T    v;
    bool showBase{};

    operator T() const { return v; }
};

template<size_t N, typename T>
radix_<N, T> radix(T v, bool showBase = false)
{
    return {v, showBase};
}

} // namespace rpav
