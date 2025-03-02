#include <catch2.hpp>

#include <rpav/iterator.hpp>
#include <rpav/log.hpp>
#include <rpav/dynarray.hpp>
#include <rpav/str/rpav_dynarray.hpp>

using namespace rpav;

using ida = dynarray<int>;

TEST_CASE("Dynarray Simple", "[dynarray]") {
    ida a = { 0, 1, 2, 3, 4 };
    ida b = a;

    REQUIRE(a.size() == 5);
    REQUIRE(a[0] == 0);
    REQUIRE(a[4] == 4);

    REQUIRE(b.size() == 5);
    REQUIRE(b[0] == 0);
    REQUIRE(b[4] == 4);
}
