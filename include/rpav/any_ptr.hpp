#pragma once

#include <rpav/ctti.hpp>
#include <rpav/str.hpp>

namespace rpav {

template<typename PTR>
class any_ptr_base {
public:
    any_ptr_base() = default;

    template<typename T>
    any_ptr_base(T *ptr) {
        _ops = ctti<T>::type();
        _ptr = ptr;
    }

    template<typename T>
    const T* get() const {
        if(ctti<T>::type() == _ops) {
            return static_cast<const T*>(_ptr);
        } else {
            return nullptr;
        }
    }

    template<typename T>
    T* get() {
        return const_cast<T*>(std::as_const(*this).template get<T>());
    }

    void toStream(std::ostream &s) const {
        if(_ptr) {
            stream(s, "<any_ptr:", _ops->_name(), ":", _ptr, ">");
        } else {
            stream(s, "<any_ptr:nullptr>");
        }
    }

    const type_ops* type() const { return _ops; }

private:
    const type_ops *_ops{nullptr};
    PTR _ptr{nullptr};
};

using any_ptr = any_ptr_base<void*>;
using const_any_ptr = any_ptr_base<const void*>;


template<typename T>
inline any_ptr make_any_ptr(T &v) {
    return any_ptr(&v);
}

template<typename T>
inline const_any_ptr make_any_ptr(const T &v) {
    return const_any_ptr(&v);
}

template<typename T>
inline void toStream(std::ostream &s, const any_ptr_base<T> &a) {
    a.toStream(s);
}

} // namespace rpav
