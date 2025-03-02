#include <rpav/str/std.hpp>
#include <catch2.hpp>

using namespace rpav;

TEST_CASE("vector", "[str_std]") {
    std::vector<int> v = { 1, 2, 3, 4 };
    std::list<int> l = { 1, 2, 3, 4 };

    std::vector<std::string> sv = { "a", "b" };
    std::list<std::string> sl = { "a", "b" };

    REQUIRE(str(v) == "[1, 2, 3, 4]");
    REQUIRE(str(l) == "(1, 2, 3, 4)");

    REQUIRE(str(sv) == "[\"a\", \"b\"]");
    REQUIRE(str(sl) == "(\"a\", \"b\")");
}
