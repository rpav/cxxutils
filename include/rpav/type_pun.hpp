#pragma once

#include <cstring>
#include <algorithm>

namespace rpav {

template<typename To, typename From>
struct type_pun_s {
    static_assert(sizeof(To) == sizeof(From) && "Punning differently-sized types");

    From &_from;
    std::remove_const_t<To> _data;

    type_pun_s(const type_pun_s &a) : _from{a._from}, _data{a._data}
    { }

    type_pun_s(From& from) : _from(from) {
        std::memcpy((void*)&_data, (const void*)&from, sizeof(From));
    }

    ~type_pun_s() {
        if constexpr(!std::is_const_v<To>) {
            std::memcpy(&_from, &_data, std::min<size_t>(sizeof(From), sizeof(To)));
        }
    }

    operator To&() { return _data; }
    operator const To&() const { return _data; }

    To& get() { return _data; }
    const To& get() const { return _data; }
};

template<typename To, typename From>
type_pun_s<To,From> pun(From& v) {
    return v;
}

}