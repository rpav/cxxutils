#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <rpav/ctti.hpp>
#include <rpav/str.hpp>
#include <rpav/typeutil.hpp>

namespace rpav {

template<size_t Width=sizeof(void*), bool DoInline=true>
class any_base {
    union {
        void *ptr;
        std::byte bytes[Width];
    } _v{nullptr};

    const type_ops *_type = nullptr;

    inline bool has_value() const {
        return _type != nullptr;
    }

    template<typename T>
    static constexpr bool is_inline() {
        return DoInline && sizeof(T) <= Width;
    }

    bool is_inline() const {
        if(DoInline && has_value())
            return _type->_sizeof() <= Width;

        return false;
    }

    template<typename T>
    constexpr T* the_ptr() {
        if constexpr(is_inline<T>())
            return (T*)_v.bytes;

        return (T*)_v.ptr;
    }

    template<typename T>
    constexpr const T* the_ptr() const {
        if constexpr(is_inline<T>())
            return (T*)_v.bytes;

        return (T*)_v.ptr;
    }

    void* the_ptr() {
        if(is_inline())
            return _v.bytes;

        return _v.ptr;
    }

    const void* the_ptr() const {
        if(is_inline())
            return _v.bytes;

        return _v.ptr;
    }

    inline void _maybe_delete() {
        if(has_value()) {
            if(is_inline())
                _type->_dtor(the_ptr());
            else
                _type->_delete(the_ptr());
        }
        _type = nullptr;
    }

    template<typename T>
    inline void _set(T &&v) {
        using ty = typename std::decay<T>::type;
        _type = ctti<ty>::type();

        if constexpr(is_inline<ty>())
            new (_v.bytes) ty(std::forward<T>(v));
        else
            _v.ptr = new ty(std::forward<T>(v));
    }

public:
    any_base() = default;

    any_base(const any_base &v) {
        _type = v._type;
        if(v.is_inline())
            _type->_copy(v._v.bytes, _v.bytes);
        else
            _v.ptr = _type->_copy(v._v.ptr);
    }

    any_base(any_base &&v) noexcept {
        _type = v._type;
        v._type = nullptr;

        if(v.is_inline())
            _type->_move(v._v.bytes, _v.bytes);
        else
            _v.ptr = v._v.ptr;
    }

    template<typename T,
        typename std::enable_if<
            !std::is_same<any_base, typename std::decay<T>::type>::value,
            bool>::type = 0
    >
    any_base(T &&v) {
        _set(std::forward<T>(v));
    }

    ~any_base() {
        _maybe_delete();
    }

    template<typename T,
        typename ty = typename std::decay<T>::type,
        typename std::enable_if<
            !std::is_same<any_base, ty>::value,
            bool>::type = 0
    >
    ty& operator= (T &&v) {
        _maybe_delete();
        _set<T>(std::forward<T>(v));
        return *(ty*)_v.ptr;
    }

    any_base& operator= (const any_base &v) {
        _maybe_delete();

        _type = v._type;
        if(v.is_inline())
            _type->_copy(v._v.bytes, _v.bytes);
        else
            _v.ptr = _type->_copy(v._v.ptr);

        return *this;
    }

    template<typename T>
    inline bool is() const {
        return (ctti<T>::type() == _type);
    }

    template<typename T>
    T* get_if() {
        if(is<T>())
            return the_ptr<T>();

        return nullptr;
    }

    template<typename T>
    const T* get_if() const {
        if(is<T>())
            return the_ptr<T>();

        return nullptr;
    }

    std::string_view name() const {
        if(has_value())
            return _type->_name();

        return "nullptr";
    }

    type_id id() const {
        if(has_value())
            return (type_id)_type;

        return 0;
    }

    const type_ops* ops() const {
        return _type;
    }

    size_t bytes() const {
        size_t sz = sizeof(*this);
        size_t szt = _type->_sizeof();

        if(is_inline())
            return sz;

        return sz + szt;
    }

    void toStream(std::ostream &s) const {
        if(has_value()) {
            _type->_tostream(s, the_ptr());
            return;
        }

        s << "#<nullptr>";
    }
};

using any = any_base<>;
using any_notinline = any_base<sizeof(void*), false>;

template<size_t Width, bool DoInline>
inline void toStream(std::ostream &s, const any_base<Width, DoInline> &a) {
    a.toStream(s);
}


class any_fun {
public:
    template<typename F>
    any_fun(F&& fn) {
        using type = decltype(make_function(fn));

        _funptr = new type(fn);
        _ops = ctti<type>::type();
    }

    any_fun(const any_fun& a) {
        _funptr = a._ops->_copy(a._funptr);
        _ops = a._ops;
    }

    any_fun(any_fun &&a) {
        _funptr = a._funptr;
        _ops = a._ops;

        a._funptr = nullptr;
        a._ops = nullptr;
    }

    ~any_fun() {
        if(_funptr)
            _ops->_delete(_funptr);
    }

    operator bool() const { return _funptr != nullptr; }

    template<typename F>
    const std::function<F>* get_if() const {
        if(_ops == ctti<std::function<F>>::type()) {
            return (std::function<F>*)_funptr;
        }

        return nullptr;
    }

    void toStream(std::ostream &s) const {
        if(_funptr)
            stream(s, "#<function<...>>");
        else
            stream(s, "#<nullptr function>");
    }

private:
    const type_ops *_ops{nullptr};
    void *_funptr{nullptr}; // note this points to some std::function<T>
};

inline void toStream(std::ostream &s, const any_fun &af) {
    af.toStream(s);
}

}
