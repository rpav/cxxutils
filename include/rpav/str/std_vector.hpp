#pragma once

#include <iomanip>
#include <vector>
#include <rpav/iterator.hpp>
#include <rpav/str.hpp>

/* Printing for std::vector */

namespace rpav {

template<typename T>
struct str_adl_vector_element {
    static void stream(std::ostream& os, const T& v) {
        toStream(os, v);
    }
};

template<>
struct str_adl_vector_element<std::string> {
    static void stream(std::ostream& os, const std::string& s) {
        os << std::quoted(s);
    }
};

template<>
struct str_adl_vector_element<std::string_view> {
    static void stream(std::ostream& os, const std::string_view& s) {
        os << std::quoted(s);
    }
};


template<typename T>
struct str_adl<std::vector<T>> {
    static void toStream(std::ostream& s, const std::vector<T>& v) {
        using namespace rpav::iter;

        stream(s, "[");

        for(auto[last, j] : zip(is_last(v), v)) {
            str_adl_vector_element<std::decay_t<T>>::stream(s, j);

            if(!last) stream(s, ", ");
        }

        stream(s, "]");
    }
};



template<typename T, size_t N>
struct str_adl<std::array<T, N>> {
    static void toStream(std::ostream& s, const std::array<T,N>& v) {
        using namespace rpav::iter;

        stream(s, "[");

        for(auto[last, j] : zip(is_last(v), v)) {
            str_adl_vector_element<std::decay_t<T>>::stream(s, j);

            if(!last) stream(s, ", ");
        }

        stream(s, "]");
    }
};
}
