#include <catch2.hpp>

#include <rpav/iterator.hpp>
#include <rpav/log.hpp>
#include <rpav/easing.hpp>

using namespace rpav;
using namespace rpav::easing;

TEST_CASE("easing_basic", "[easing]") {
    REQUIRE(linear(0.0) == 0.0);
    REQUIRE(linear(0.5) == 0.5);
    REQUIRE(linear(1.0) == 1.0);

    REQUIRE(quadIn(0.0) == 0.0);
    REQUIRE(quadIn(0.25) == 0.0625);
    REQUIRE(quadIn(0.5) == 0.25);
    REQUIRE(quadIn(0.75) == 0.5625);
    REQUIRE(quadIn(1.0) == 1.0);

    REQUIRE(quadOut(0.0) == 0.0);
    REQUIRE(quadOut(0.25) == 0.4375);
    REQUIRE(quadOut(0.5) == 0.75);
    REQUIRE(quadOut(0.75) == 0.9375);
    REQUIRE(quadOut(1.0) == 1.0);

    REQUIRE(quadInOut(0.0) == 0.0);
    REQUIRE(quadInOut(0.25) == 0.125);
    REQUIRE(quadInOut(0.5) == 0.5);
    REQUIRE(quadInOut(0.75) == 0.875);
    REQUIRE(quadInOut(1.0) == 1.0);
}

TEST_CASE("easing_sine", "[easing]") {
    REQUIRE(sineIn(0.0f) == Approx(0.0));
    REQUIRE(sineIn(0.25f) == Approx(0.0761205));
    REQUIRE(sineIn(0.5f) == Approx(0.292893));
    REQUIRE(sineIn(0.75f) == Approx(0.617317));
    REQUIRE(sineIn(1.0f) == Approx(1.0));

    REQUIRE(sineOut(0.0f) == Approx(0.0));
    REQUIRE(sineOut(0.25f) == Approx(0.382683));
    REQUIRE(sineOut(0.5f) == Approx(0.707107));
    REQUIRE(sineOut(0.75f) == Approx(0.92388));
    REQUIRE(sineOut(1.0f) == Approx(1.0));
}

TEST_CASE("easing_circ", "[easing]") {
    REQUIRE(circOut(0.0f) == Approx(0.0));
    REQUIRE(circOut(0.25f) == Approx(0.661438));
    REQUIRE(circOut(0.5f) == Approx(0.866025));
    REQUIRE(circOut(0.75f) == Approx(0.968246));
    REQUIRE(circOut(1.0f) == Approx(1));
}

TEST_CASE("easing_expo", "[easing]") {
    REQUIRE(expoOut(0.0f) == Approx(0.0));
    REQUIRE(expoOut(0.25f) == Approx(0.823223));
    REQUIRE(expoOut(0.5f) == Approx(0.96875));
    REQUIRE(expoOut(0.75f) == Approx(0.994476));
    REQUIRE(expoOut(1.0f) == Approx(0.999023));

    REQUIRE(expoInOut(0.0f) == Approx(0.000488281));
    REQUIRE(expoInOut(0.25f) == Approx(0.015625));
    REQUIRE(expoInOut(0.5f) == Approx(0.5));
    REQUIRE(expoInOut(0.75f) == Approx(0.984375));
    REQUIRE(expoInOut(1.0f) == Approx(0.999512));
}
