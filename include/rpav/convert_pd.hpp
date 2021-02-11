#pragma once

#include <utility>

#include <piped.hpp>
#include <rpav/ptr.hpp>
#include <rpav/convert.hpp>

namespace rpav {

template<typename To>
struct piped_to {
    template<typename From>
    To process(From&& v) const
    {
        return conv_adl<std::decay_t<remove_ref_t<From>>, To>::convert(std::forward<From>(v));
    }
};

template<typename To>
constexpr piped_to<To> to_{};

struct piped_auto_convert {
    template<typename From>
    auto_convert<From> process(const From& v) const
    {
        return {v};
    }
};

constexpr piped_auto_convert conv;

} // namespace rpav
