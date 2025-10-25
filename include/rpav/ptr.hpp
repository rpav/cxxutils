#pragma once

#include <memory>
#include <stdexcept>

#include <rpav/config.hpp>
#include <rpav/ctti.hpp>
#include <rpav/str.hpp>

namespace rpav {

template<typename T, typename D = std::default_delete<T>>
using uptr = std::unique_ptr<T, D>;

template<typename T, typename... Rest>
inline std::unique_ptr<T> make(Rest&&... r)
{
    return std::make_unique<T>(std::forward<Rest>(r)...);
}

class nullptr_error : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

// This is useful because, in no particular order:
//  1. nullptr default
//  2. optional cross-platform null-deref checking
//  3. const clarity!
template<typename T>
class ptr {
    template<typename U>
    friend class ptr;

public:
    using type       = T*;
    using value_type = T;

protected:
    T* _ptr{nullptr};

    static inline T* ensure_ptr(T* p)
    {
        if constexpr(config::ptrCheck) {
            if(!p) {
                throw nullptr_error{
                    rpav::str("Null pointer dereference for ptr<", ctti<T>::name(), ">")};
            }
        }
        return p;
    }

    inline T* ensure_ptr() const { return ensure_ptr(_ptr); }
    inline T* ensure_ptr() { return ensure_ptr(_ptr); }

public:
    constexpr ptr() = default;
    constexpr ptr(T* p) : _ptr(p) {}
    constexpr ptr(const ptr&) = default;
    constexpr ptr(ptr&&)      = default;

    template<typename S, typename = std::enable_if_t<not std::is_same_v<S, T>>>
    constexpr ptr(const ptr<S>& p) : _ptr(p._ptr)
    {}

    T& operator*() const { return *ensure_ptr(); }
    T& operator*() { return *ensure_ptr(); }

    T* operator->() const { return ensure_ptr(); }
    T* operator->() { return ensure_ptr(); }

    constexpr explicit operator bool() { return _ptr != nullptr; }
    constexpr bool     is_null() { return _ptr == nullptr; }

    T* get() const { return _ptr; }

    operator T*() const { return _ptr; }

    template<typename S, std::enable_if_t<std::is_same_v<S, T> && !std::is_const_v<S>>>
    operator const T*() const
    {
        return _ptr;
    }

    ptr& operator=(const ptr&)     = default;
    ptr& operator=(ptr&&) noexcept = default;

    bool operator==(const ptr& p) const { return _ptr == p._ptr; }
    bool operator!=(const ptr& p) const { return _ptr != p._ptr; }

    constexpr bool operator==(std::nullptr_t) const { return _ptr == nullptr; }
    constexpr bool operator!=(std::nullptr_t) const { return _ptr != nullptr; }

    template<typename S>
    bool operator==(S* p) const
    {
        return _ptr == p;
    }

    template<typename S>
    bool operator!=(S* p) const
    {
        return _ptr != p;
    }
};

template<>
class ptr<void> {
    template<typename U>
    friend class ptr;

public:
    using type = void*;

protected:
    type _ptr{nullptr};

public:
    constexpr ptr() = default;
    constexpr ptr(type p) : _ptr(p) {}
    constexpr ptr(const ptr&) = default;
    constexpr ptr(ptr&&)      = default;

    template<typename S>
    constexpr ptr(const ptr<S>& p) : _ptr(p._ptr)
    {}

    constexpr explicit operator bool() { return _ptr != nullptr; }
    constexpr bool     is_null() { return _ptr == nullptr; }

    type get() const { return _ptr; }

    operator type() { return _ptr; }

    ptr& operator=(const ptr&) = default;
    ptr& operator=(ptr&&)      = default;

    bool operator==(const ptr& p) const { return _ptr == p._ptr; }
    bool operator!=(const ptr& p) const { return _ptr != p._ptr; }

    template<typename S>
    bool operator==(S* p) const
    {
        return _ptr == p;
    }
    template<typename S>
    bool operator!=(S* p) const
    {
        return _ptr != p;
    }
};

/// Box `T`, i.e. allocate it on the heap and box<T> is a pointer.
/// Acts more like a value than a unique_ptr, i.e. if you copy it, it copies its data.
// TODO: support custom allocation?
template<typename T>
class box {
    T* _data{};

    inline void maybe_free()
    {
        if(_data) delete _data;
    }

public:
    box() : _data{new T()} {}

    template<typename S>
    box(S* v) noexcept : _data(v)
    {}

    box(const box& b) { _data = new T(*b._data); }
    box(box&& b) noexcept
    {
        _data   = b._data;
        b._data = nullptr;
    }

    template<typename... Ts>
    box(Ts&&... ts) : _data{new T(std::forward<Ts>(ts)...)}
    {}

    ~box() { maybe_free(); }

    T*       operator->() { return _data; }
    const T* operator->() const { return _data; }

    T&       operator*() { return *_data; }
    const T& operator*() const { return *_data; }

    operator T*() { return _data; }
    operator const T*() const { return _data; }

    T&       get() { return *_data; }
    const T& get() const { return *_data; }

    template<typename S>
    box& operator=(S&& s) noexcept
    {
        *_data = std::move(s);
        return *this;
    }

    box& operator=(const box& b)
    {
        *_data = *b._data;
        return *this;
    }

    box& operator=(box&& b) noexcept
    {
        maybe_free();
        _data   = b._data;
        b._data = nullptr;
        return *this;
    }
};

// This is basically a `not_null<T>`, but `ref<T>` is shorter.
// This is useful because:
//   1. Reassignment
//   2. Less weirdness in structs
//   3. const clarity!
//   4. `auto x = ...` is correct between value, ptr<T>, and ref<T>
// Not-really-disadvantages:
//   1. You have to use -> .. but then, always consistency for referential types
template<typename T>
class ref {
    template<typename U>
    friend class ref;

public:
    using type       = T;
    using value_type = T;

protected:
    T* _ptr{nullptr};

public:
    constexpr ref() = delete;
    constexpr ref(T& v) : _ptr(&v) {}
    constexpr ref(const ref&) = default;
    constexpr ref(ref&&)      = default;

    template<typename S, typename = std::enable_if_t<not std::is_same_v<S, T>>>
    constexpr ref(const ref<S>& p) : _ptr(p._ptr)
    {}

    T& operator*() const { return *_ptr; }
       operator T&() const { return *_ptr; }
    T* operator->() const { return _ptr; }

    template<typename S, std::enable_if_t<std::is_same_v<S, T> && !std::is_const_v<S>>>
    operator const S&() const
    {
        return _ptr;
    }

    T* get() const { return _ptr; }

    T* operator&() const { return _ptr; }

    ref& operator=(const ref&) = default;
    ref& operator=(ref&&)      = default;

    bool is(const ref& v) const { return _ptr == &v; }
};

template<
    template<typename...> typename T,
    typename... Ts,
    typename = std::enable_if_t<std::is_same_v<T<Ts...>, ref<Ts...>>>>
ref(T<Ts...>&) -> ref<Ts...>;

template<typename T>
struct remove_ref {
    using type = std::remove_reference_t<T>;
};

template<typename T>
struct remove_ref<ref<T>> {
    using type = std::remove_reference_t<T>;
};

template<typename T>
using remove_ref_t = typename remove_ref<T>::type;

template<typename T>
struct str_adl<ref<T>> {
    static inline void toStream(std::ostream& os, const ref<T>& r) { stream(os, *r); }
};

} // namespace rpav
