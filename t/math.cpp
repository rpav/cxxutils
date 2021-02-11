#include <rpav/math.hpp>
#include <catch2/catch.hpp>

using namespace rpav;

TEST_CASE("math/basic", "[math]") {
    // Yeah not the most specific test
    REQUIRE((math::pi > 3.14f && math::pi < 3.15));
}
