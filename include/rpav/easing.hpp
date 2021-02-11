#pragma once

/*
 * Super trivial easing functions: in fact, they expect only a
 * normalized `t` input and return only a normalized output.
 * Scaling and translation is up to you (but also trivial).
 */

#include <cmath>
#include <rpav/math.hpp>

#define RPAV_EASE(fun, n) \
template<typename F> inline F fun##In(F t) { return in<F>(fun,t,(n)); } \
template<typename F> inline F fun##Out(F t) { return out<F>(fun,t,(n)); } \
template<typename F> inline F fun##InOut(F t) { return inout<F>(fun,t,(n)); }

namespace rpav::easing {

template<typename F>
using fun = F(F);

template<typename F> inline F in(fun<F> f, F t, F = 1.0) { return f(t); }
template<typename F> inline F out(fun<F> f, F t, F neg = 1.0) { return neg * f(t-1) + 1; }

template<typename F>
inline F inout(fun<F> f, F t, F neg = 1.0) {
    return t < 0.5 ?
           0.5 * in(f,2*t,neg) :
           0.5 * out(f,2*t-1,neg) + 0.5;
}

template<typename F> inline F sine(F t) { return -std::cos(t*math::K<F>::pi_2)+1; }
template<typename F> inline F linear(F t) { return t; }
template<typename F> inline F quad(F t) { return t*t; }
template<typename F> inline F cubic(F t) { return t*t*t; }
template<typename F> inline F quart(F t) { return t*t*t*t; }
template<typename F> inline F quint(F t) { return t*t*t*t*t; }
template<typename F> inline F circ(F t) { return 1-std::sqrt(1-t*t); }

RPAV_EASE(sine, -1)
RPAV_EASE(quad, -1)
RPAV_EASE(cubic, 1)
RPAV_EASE(quart, -1)
RPAV_EASE(quint, 1)
RPAV_EASE(circ, -1)

template<typename F> inline F expoIn(F t) { return std::pow((F)2,(F)10*(t-1)); }
template<typename F> inline F expoOut(F t) { return -std::pow((F)2,(F)-10*t)+1; }
template<typename F> inline F expoInOut(F t) {
    return t < 0.5 ?
           0.5 * expoIn(2*t) :
           0.5 * expoOut(2*t-1) + 0.5;
}

template<typename F> inline F elastic(F t) {
    auto t2 = t*t;
    auto t3 = t*t*t;
    return (33*t3*t2 - 106*t2*t2 + 126*t3 - 67*t2 + 15*t);
}

template<typename F> inline F triangle(F t) {
    auto m2 = fmodf(t, 2.0);
    return m2 < 1.0 ? m2 : 2-m2;
}

}
