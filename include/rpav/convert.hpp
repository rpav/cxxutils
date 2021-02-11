#pragma once

namespace rpav {

// Default to whatever static_cast gives us
// Provide specific implementation for more.
template<typename From, typename To>
struct conv_adl {
    static To convert(const From& v) {
        To x = v;
        return x;
    }
};

// Semi-explicit conversion operator for pass-through.
template<typename T>
struct auto_convert {
    const T& v;

    template<typename To>
    operator To() { return conv_adl<T,To>::convert(v); }
};

template<typename To, typename From>
inline To to(const From& v) { return conv_adl<From,To>::convert(v); }


/* For better auto conversion, define this in your class with an explicit FROM:

    template<typename To>
    explicit operator To() const { return rpav::conv_adl<FROM,To>::convert(*this); }
 */

template<typename From>
inline auto_convert<From> cnv(const From& v) {
    return {v};
}

}
