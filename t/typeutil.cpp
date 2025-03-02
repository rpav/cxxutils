#include <rpav/typeutil.hpp>
#include <rpav/log.hpp>

#include <catch2.hpp>

using namespace rpav;


struct A { static constexpr int value = 0; };
struct B { static constexpr int value = 1; };

template<typename, typename S = void>
struct X {
    static constexpr int value = 42;

    using type = typename or_type<S, X>::type;
};


TEST_CASE("or_type", "[typeutil]") {
    REQUIRE(or_type<A,B>::type::value == 0);
    REQUIRE(or_type<void,B>::type::value == 1);
    REQUIRE(X<int>::type::value == 42);
    REQUIRE(X<int,B>::type::value == 1);
}
