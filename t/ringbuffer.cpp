#include <rpav/ringbuffer.hpp>
#include <rpav/str/rpav_ringbuffer.hpp>
#include <catch2.hpp>

using namespace rpav;

ringbuffer<int> setup() {
    ringbuffer<int> r(3);

    r.emplace_back(0);
    r.emplace_back(1);
    r.emplace_back(2);
    r.emplace_back(3);
    r.emplace_back(4);
    r.emplace_back(5);

    return r;
}

TEST_CASE("Ringbuffer Simple", "[ringbuffer]") {
    auto r = setup();

    REQUIRE(r.size() == 3);
    REQUIRE(r[0] == 3);
    REQUIRE(r[1] == 4);
    REQUIRE(r[2] == 5);

    REQUIRE(r.front() == 3);
    REQUIRE(r.back() == 5);
}

TEST_CASE("Ringbuffer Iterator", "[ringbuffer]") {
    auto r = setup();

    auto i = r.begin();
    REQUIRE(*i == 3); ++i;
    REQUIRE(*i == 4); ++i;
    REQUIRE(*i == 5); ++i;
    REQUIRE(i == r.end());

    auto ri = r.rbegin();
    REQUIRE(*ri == 5); ++ri;
    REQUIRE(*ri == 4); ++ri;
    REQUIRE(*ri == 3); ++ri;
    REQUIRE(ri == r.rend());
}

TEST_CASE("Ringbuffer Pop", "[ringbuffer]") {
    auto r = setup();

    r.pop_back(); REQUIRE(r.back() == 4);
    r.pop_back(); REQUIRE(r.back() == 3);
}
