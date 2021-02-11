#pragma once

#include <cstdint>
#include <cstddef>

#include <rpav/typeutil.hpp>
#include <rpav/value.hpp>

namespace rpav {

using uint8  = value<uint8_t>;
using uint16 = value<uint16_t>;
using uint32 = value<uint32_t>;
using uint64 = value<uint64_t>;

using int8  = value<int8_t>;
using int16 = value<int16_t>;
using int32 = value<int32_t>;
using int64 = value<int64_t>;

template<> struct is_signed<int8> { static constexpr bool value = true; };
template<> struct is_signed<int16> { static constexpr bool value = true; };
template<> struct is_signed<int32> { static constexpr bool value = true; };
template<> struct is_signed<int64> { static constexpr bool value = true; };

using size    = value<size_t>;
using ptrdiff = value<ptrdiff_t>;

using float32 = value<float>;
using float64 = value<double>;

template<> struct is_signed<float32> { static constexpr bool value = true; };
template<> struct is_signed<float64> { static constexpr bool value = true; };

using boolv = value<bool>;

inline uint8_t operator""_u8(unsigned long long u) { return u; }
inline uint16_t operator""_u16(unsigned long long u) { return u; }
inline uint32_t operator""_u32(unsigned long long u) { return u; }
inline uint64_t operator""_u64(unsigned long long u) { return u; }

inline int8_t operator""_i8(unsigned long long u) { return u; }
inline int16_t operator""_i16(unsigned long long u) { return u; }
inline int32_t operator""_i32(unsigned long long u) { return u; }
inline int64_t operator""_i64(unsigned long long u) { return u; }

inline float operator""_f(long double f) { return f; }
inline double operator""_d(long double f) { return f; }

inline float operator""_f32(long double f) { return f; }
inline double operator""_f64(long double f) { return f; }

inline size_t operator""_sz(unsigned long long u) { return u; }
inline size_t operator""_uz(unsigned long long u) { return u; }
inline ssize_t operator""_z(unsigned long long u) { return u; }

// "prefix literals" e.g. `i8_<0>`
// The benefit is these are validation-aware: `i8_<128>` is an error.

template<auto V> auto i8_ = value_lit<int8, V>::value;

} // namespace rpav
