#include <catch2.hpp>

#include <rpav/modify_track.hpp>

using namespace rpav;

TEST_CASE("Modify track simple", "[modify_track]") {
    modify_track<int> m{42};

    REQUIRE(m.isModified());
    REQUIRE(m == 42);
    REQUIRE(*m == 42);

    m.reset();

    REQUIRE(!m.isModified());

    auto &i = m.get();

    REQUIRE(m.isModified());

    i = 43;

    REQUIRE(i == 43);
    REQUIRE(m == 43);
}
