#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <rpav/enum.hpp>

/* Adapted from some public domain code */
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64))
    #define __BIG_ENDIAN 4321
    #define __LITTLE_ENDIAN 1234
    #define __PDP_ENDIAN 3412

    // Assume little endian, because windows.h / windef.h etc
    // break too much. (near/far/min/max...)
    #define __BYTE_ORDER __LITTLE_ENDIAN
#elif defined(__linux__) || defined(__CYGWIN__)

    #include <endian.h>

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
    #include <sys/endian.h>
#elif defined(__APPLE__)
    #include <libkern/OSByteOrder.h>
    #define __BYTE_ORDER BYTE_ORDER
    #define __BIG_ENDIAN BIG_ENDIAN
    #define __LITTLE_ENDIAN LITTLE_ENDIAN
    #define __PDP_ENDIAN PDP_ENDIAN
#else
    #error "Platform endianness cannot be determined."
#endif

namespace rpav {

struct endian : public enum_class<endian, uint8_t> {
    enum Value {
        little,
        big,
    };

    endian() : endian(native) { }
    ENUM(endian);

    static constexpr Value native{
        (__BYTE_ORDER == __BIG_ENDIAN) ? big : little
    };
};

template<typename T>
inline T byte_reverse(const T &val) {
    if constexpr(sizeof(T) == 1) {
        return val;
    }

    T v{val};
    auto *b = (std::byte *) &v;
    std::reverse(&b[0], &b[sizeof(T)-1]);

    return v;
}

template<typename T>
class byte_access {
    const T &_val;

public:
    byte_access(const T &v) : _val(v) { }

    constexpr size_t size() const { return sizeof(T); }

    /**
     * Access bytes uniformly, as if `T` were little-endian
     * @param i ith LSB
     * @return Requested byte
     */
    inline
    std::byte operator[](size_t i) const {
        auto *b = (std::byte *) &_val;

        if constexpr(endian::native == endian::big) {
            return b[sizeof(T) - i - 1];
        } else {
            return b[i];
        }
    }
};

}