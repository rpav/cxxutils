#include <rpav/iterator.hpp>
#include <rpav/span.hpp>
#include <rpav/str/rpav.hpp>
#include <rpav/log.hpp>
#include <rpav/util.hpp>
#include <catch2.hpp>

using namespace rpav;

TEST_CASE("span/simple", "[span]") {
    int arr[] = { 10, 20, 30, 40 };
    span s(arr);

    REQUIRE(s.size() == 4);
    REQUIRE(s.front() == 10);
    REQUIRE(s.back() == 40);
    REQUIRE(!s.empty());

    span t(arr, size_t(0));
    REQUIRE(t.size() == 0);
    REQUIRE(t.empty());
}


TEST_CASE("span/reverse iterator", "[span]") {
    int arr[] = { 1, 2, 3, 4 };
    span s(arr);

    REQUIRE(s.rbegin().base().get() == (arr + 4));
    REQUIRE(s.rend().base().get() == arr);
    REQUIRE(*s.rbegin() == 4);
}
