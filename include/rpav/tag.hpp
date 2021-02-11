#pragma once

#include <rpav/str.hpp>

#define Tag(N)                                                                                                         \
    template<typename T>                                                                                               \
    struct N {                                                                                                         \
        T v;                                                                                                           \
        N() = default;                                                                                                 \
        template<typename S>                                                                                           \
        N(S v_) : v{std::move(v_)}                                                                                     \
        {}                                                                                                             \
                 operator T&() { return v; }                                                                           \
                 operator const T&() const { return v; }                                                               \
        T*       operator->() { return &v; }                                                                           \
        const T* operator->() const { return &v; }                                                                     \
    };                                                                                                                 \
    template<typename T>                                                                                               \
    N(T) -> N<T>;                                                                                                      \
    template<typename T>                                                                                               \
    void toStream(std::ostream& os, const N<T>& v)                                                                     \
    {                                                                                                                  \
        stream(os, #N "(", v.v, ")");                                                                                  \
    }

namespace rpav {}
namespace rpav::tag {}

#define StrongAlias(NAME, SUPER)                                                                                       \
    struct NAME : public SUPER {                                                                                       \
        using SUPER::SUPER;                                                                                            \
        NAME(const SUPER& v) { *this = v; }                                                                            \
        NAME(SUPER&& v) { *this = std::move(v); }                                                                      \
        NAME& operator=(const SUPER& v)                                                                                \
        {                                                                                                              \
            this->SUPER::operator=(v);                                                                                 \
            return *this;                                                                                              \
        }                                                                                                              \
    }

#define StrongAliasNS(NAME, NS, SUPER)                                                                                 \
    struct NAME : public NS::SUPER {                                                                                   \
        using SUPER::SUPER;                                                                                            \
        NAME(const SUPER& v) { *this = v; }                                                                            \
        NAME(SUPER&& v) { *this = std::move(v); }                                                                      \
        NAME& operator=(const SUPER& v)                                                                                \
        {                                                                                                              \
            this->SUPER::operator=(v);                                                                                 \
            return *this;                                                                                              \
        }                                                                                                              \
    }
