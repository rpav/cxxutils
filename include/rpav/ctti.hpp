#pragma once

/* Some compile-time and run-time type info.
 */

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <ostream>

#if defined(__GNUC__) && !defined(__clang__)
    #ifndef __GNUC_PREREQ
        #define __GNUC_PREREQ(MAJOR, MINOR) ((__GNUC__ >= MAJOR) && (__GNUC_MINOR >= MINOR))
    #endif
#endif

namespace rpav {

using type_id = std::uintptr_t;

template<typename T>
inline void toStream(std::ostream &os, const T&);

struct type_ops {
    [[nodiscard]] virtual void* _copy(const void *ptr) const = 0;
    virtual void _copy(const void *src, void *dest) const = 0;
    virtual void _move(const void *src, void *dest) const = 0;

    virtual void _delete(void*) const = 0;
    virtual void _dtor(void*) const = 0;
    virtual size_t _sizeof() const = 0;

    virtual size_t _align_of() const = 0;
    virtual std::string_view _name() const = 0;

    virtual type_id _id() const = 0;
    virtual bool _is(type_id) const = 0;

    virtual void _tostream(std::ostream &, const void*) const = 0;

    template<typename T>
    inline bool is() const;
};

template<typename T>
struct type_impl : public type_ops {
    [[nodiscard]] void* _copy(const void *ptr) const override {
        static_assert(!std::is_reference<T>::value, "Cannot copy a pointer-to-reference");
        assert(ptr && "_copy called on nullptr");
        return new typename std::remove_const<T>::type( * static_cast<const T*>(ptr) );
    }

    void _copy(const void *ptr, void *placement) const override {
        static_assert(!std::is_reference<T>::value, "Cannot copy a pointer-to-reference");
        assert(ptr && "_copy called on nullptr");
        assert(placement && "_copy called with nullptr placement");
        new (placement) T(*(T*)ptr);
    }

    void _move(const void *ptr, void *placement) const override {
        static_assert(!std::is_reference<T>::value, "Cannot move a pointer-to-reference");
        assert(ptr && "_move called on nullptr");
        assert(placement && "_move called with nullptr placement");
        new (placement) T(std::move(*(T*)ptr));
    }

    void _delete(void *ptr) const override {
        delete (T*)ptr;
    }

    void _dtor(void *ptr) const override {
        ((T*)ptr)->~T();
    }

    size_t _sizeof() const override {
        return sizeof(T);
    }

    size_t _align_of() const override {
        return alignof(T);
    }

    std::string_view _name() const override;

    type_id _id() const override;
    bool _is(type_id) const override;

    void _tostream(std::ostream &s, const void *ptr) const override {
        toStream(s, *(const T*)ptr);
    }
};

template<typename T>
struct ctti_base {
    static inline const struct type_ops* type() {
        const static type_impl<T> a{};
        return &a;
    }
    static inline type_id id() { return (type_id)type(); }
};


// Adapted from:
// Howard Hinnet
// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/20170989#20170989
// MIT License

template<typename T>
struct typename_adl {
    static inline constexpr std::string_view name()
    {
        using namespace std;
#ifdef __clang__
        // checked with clang-7.0.0
        string_view p = __PRETTY_FUNCTION__;
        constexpr int len = 43;
        auto sv = string_view(p.data() + len, p.find('=',len) - len - 13);
        sv.remove_suffix(sv.size() - sv.find_last_not_of(' ') - 1);
        return sv;
#elif defined(__GNUC__)
        string_view p = __PRETTY_FUNCTION__;

#  if __GNUC_PREREQ(7, 0)
        // checked with gcc 7.3, gcc 8.1
        constexpr int len = 74;
        return string_view(p.data() + len, p.find(';', len) - len);
#  elif __cplusplus < 201402
        // not checked
        return string_view(p.data() + 36, p.size() - 36 - 1);
#  endif
#elif defined(_MSC_VER)
        // not checked
        string_view p = __FUNCSIG__;
        constexpr int len = 93;
        return string_view(p.data() + len, p.size() - len - 13);
#endif
    }
};

template<typename T>
struct ctti final : public ctti_base<T> {
    // TODO: Remove all pointers/etc, and preferably return them as part of the typename
    static constexpr std::string_view name() { return typename_adl<T>::name(); }
};

template<typename T>
bool type_ops::is() const {
    return this->_is(ctti<T>::id());
}

template<typename T>
std::string_view type_impl<T>::_name() const {
    return ctti<T>::name();
}

template<typename T>
type_id type_impl<T>::_id() const {
    return ctti<T>::id();
}

template<typename T>
bool type_impl<T>::_is(type_id id) const {
    return id == ctti<T>::id();
}

template<typename T>
std::string_view name_of(const T&) { return ctti<T>::name(); }

template<typename T>
std::string_view name_of() { return ctti<T>::name(); }

} // namespace rpav
