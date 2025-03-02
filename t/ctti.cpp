#include <catch2.hpp>

#include <rpav/ctti.hpp>
#include <rpav/log.hpp>

using namespace rpav;

namespace foo {
template<typename T>
struct Bar { T v; };
}

TEST_CASE("ctti_typename", "[ctti]") {
    REQUIRE(ctti<int>::name() == "int");
    REQUIRE(ctti<foo::Bar<int>>::name() == "foo::Bar<int>");
    REQUIRE(ctti<char[4]>::name() == "char [4]");

    // Note this can differ between clang/gcc and versions; should add some per-compiler testing
    REQUIRE(ctti<foo::Bar<foo::Bar<int>>>::name() == "foo::Bar<foo::Bar<int> >");
}
