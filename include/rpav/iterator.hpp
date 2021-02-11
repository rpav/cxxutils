#pragma once

#include <cstdint>
#include <cstddef>
#include <iterator>

namespace rpav {

template<
    typename Category,
    typename T,
    typename Distance = std::ptrdiff_t,
    typename Pointer = T*,
    typename Reference = T&
>
struct iterator_def {
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

}

#include <rpav/typeutil.hpp>
#include <rpav/iterator/adaptor.hpp>
#include <rpav/iterator/reverse.hpp>
#include <rpav/iterator/with_iterator.hpp>
#include <rpav/iterator/last.hpp>
#include <rpav/iterator/zip.hpp>
#include <rpav/iterator/from.hpp>
#include <rpav/iterator/misc.hpp>
