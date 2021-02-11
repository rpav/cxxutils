#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>

#include <rpav/typeutil.hpp>

namespace rpav::math {

// Because apparently the standards committee hasn't bothered to get
// around to pi
template<typename F>
struct K {
    static constexpr F pi = F(
        3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651);
    static constexpr F pi_2 = (pi / F(2));
};

constexpr auto pi   = K<float>::pi;
constexpr auto pi_d = K<double>::pi;

constexpr auto pi_2   = (pi / 2.0f);
constexpr auto pi_d_2 = (pi_d / 2.0);

constexpr auto tau   = 2.0f * pi;
constexpr auto tau_d = 2.0 * pi_d;

/**
 * Return `-1` or `1` corresponding to sign of `v`.  This is not defined for
 * unsigned types.
 * @tparam T Signed type.
 * @param v Sign value to check
 * @return T(-1) if `v < 0`, or T(1)
 */
template<typename T, typename = std::enable_if_t<rpav::is_signed_v<T>>>
inline T signof(T v)
{
    return ((v < T(0)) * T(-2)) + T(1);
}

/**
 * Return `-1`, `0`, or `1` corresponding to sign of `v` (or 0).  This is not defined for
 * unsigned types.
 * @tparam T Signed type.
 * @param v Sign-or-zero value to check
 * @return `T(-1)` if `v < 0`, `T(0)` if `v == 0`, or `T(1)`
 */
template<typename T, typename = std::enable_if_t<rpav::is_signed_v<T>>>
inline T sign_or_zero(T v)
{
    return -T(v < T(0)) + T(v > T(0));
}

// Safer than std::minmax
template<typename T>
inline std::pair<T, T> minmax(T&& a, T&& b)
{
    if(a > b) return {b, a};
    return {a, b};
}

// Better than std::min/max

// </3
#if(defined(_WIN16) || defined(_WIN32) || defined(_WIN64))
#    undef min
#    undef max
#endif

template<typename T>
inline T min(T a, T b)
{
    return (b > a ? a : b);
}

template<typename T, typename... Ts>
inline T min(T a, T b, Ts&&... ts)
{
    return (b > a ? min(a, ts...) : min(b, ts...));
}

template<typename T>
inline T max(T a, T b)
{
    return (b > a ? b : a);
}

template<typename T, typename... Ts>
inline T max(T a, T b, Ts&&... ts)
{
    return (b > a ? max(b, ts...) : max(a, ts...));
}

template<typename T>
inline T abs(T f) {
    return T(std::abs(f));
}

/**
 * Clamp `v` to a value between `a` and `b`.
 *
 * @param v Value to clamp
 * @param a High or low of range to clamp
 * @param b High or low of range to clamp
 * @returns Value no higher than `max(a,b)` and no lower than `min(a,b)`
 */
template<typename T, typename S0, typename S1>
inline T clamp(T v, S0 a, S1 b)
{
    return min<T>(b, max<T>(a, v));
}

/**
 * Like `clamp` but assigns the result to `v` instead of simply returning it.
 */
template<typename T, typename S0, typename S1>
inline T& clampset(T& v, S0 a, S1 b)
{
    return v = clamp(v, a, b);
}

/// Equal-with-epsilon
template<typename T>
inline bool equal(T a, T b, T epsilon)
{
    return abs(b - a) < epsilon;
}

/// Less-than-with-epsilon
template<typename T, typename F>
inline bool lt(T a, T b, F epsilon)
{
    auto aa = abs(a);
    auto ab = abs(b);

    return (b - a) > (aa < ab ? ab : aa) * epsilon;
}

template<typename T, typename F>
inline bool lte(T a, T b, F epsilon)
{
    auto aa = abs(a);
    auto ab = abs(b);

    return (b - a) > (aa < ab ? ab : aa) * epsilon || equal(a, b, T(epsilon));
}

/// Greater-than-with-epsilon
template<typename T, typename F>
inline bool gt(T a, T b, F epsilon)
{
    auto aa = abs(a);
    auto ab = abs(b);

    return (b - a) < (aa > ab ? ab : aa) * epsilon;
}

template<typename T, typename F>
inline bool gte(T a, T b, F epsilon)
{
    auto aa = abs(a);
    auto ab = abs(b);

    return (b - a) < (aa > ab ? ab : aa) * epsilon || equal(a, b, T(epsilon));
}

/**
 * Test whether `a <= v <= b`.
 */
template<typename T>
inline bool between(T v, T a, T b)
{
    return (v >= a) && (v <= b);
}

/* Because seriously why not */

/// Set the `nth` bit of `v`
template<typename T>
inline void setBit(T& v, uint8_t nth)
{
    v |= (T(1) << (nth - 1));
}

/// Clear the `nth` bit of `v`
template<typename T>
inline void clearBit(T& v, uint8_t nth)
{
    v &= ~(T(1) << (nth - 1));
}

/// Assign `b` to `nth` bit of `v`
template<typename T>
inline void assignBit(T& v, uint8_t nth, bool b)
{
    b ? setBit(v, nth) : clearBit(v, nth);
}

template<typename T>
inline T degToRad(const T&& v)
{
    return v * (K<T>::pi / T(180));
}

} // namespace rpav::math
