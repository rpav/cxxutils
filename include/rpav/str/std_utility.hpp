#pragma once

#include <iomanip>
#include <utility>
#include <tuple>

#include <rpav/str.hpp>

namespace rpav {

template<typename T>
struct str_adl_pair_element {
    static void toStream(std::ostream& os, const T& v)
    {
        rpav::toStream(os, v);
    }
};

template<>
struct str_adl_pair_element<std::string> {
    static void toStream(std::ostream& os, const std::string& s)
    {
        os << std::quoted(s);
    }
};

template<>
struct str_adl_pair_element<std::string_view> {
    static void toStream(std::ostream& os, const std::string_view& s)
    {
        os << std::quoted(s);
    }
};

template<typename A, typename B>
struct str_adl<std::pair<A, B>> {
    static void toStream(std::ostream& s, const std::pair<A, B>& p)
    {
        stream(s, "{");
        str_adl_pair_element<std::decay_t<A>>::toStream(s, p.first);
        stream(s, ": ");
        str_adl_pair_element<std::decay_t<B>>::toStream(s, p.second);
        stream(s, "}");
    }
};

template<typename...Ts>
struct str_adl<std::tuple<Ts...>> {
    using tuple = std::tuple<Ts...>;
    static constexpr auto size = sizeof...(Ts);

    static void toStream(std::ostream& s, const tuple& t)
    {
        stream(s, "<");
        strEl<>(s, t);
        stream(s, ">");
    }

private:
    template<size_t N=0>
    static void strEl(std::ostream& s, const tuple& t) {
        str_adl_pair_element<std::decay_t<std::tuple_element_t<N, tuple>>>::toStream(s, std::get<N>(t));

        if constexpr (size != 1 && N < (size - 1)) {
            stream(s, ", ");
            strEl<N+1>(s, t);
        }
    }
};

template<typename T, typename...Ts>
struct str_adl<std::unique_ptr<T, Ts...>> {
    using type = std::unique_ptr<T,Ts...>;

    static void toStream(std::ostream& s, const type& p) {
        stream(s, ctti<type>::name(), "(", p.get(), ")");
    }
};

template<>
struct str_adl<std::byte> {
    static void toStream(std::ostream& s, const std::byte& b) {
        stream(s, (int)b);
    }
};

}
