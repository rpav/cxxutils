#include <rpav/ptr.hpp>
#include <rpav/log.hpp>
#include <catch2/catch.hpp>

using namespace rpav;

TEST_CASE("ptr/simple", "[ptr]") {
    int i = 42;

    auto p = ptr<int>(&i);
    auto pc = ptr<const int>(&i);
    const auto cp = ptr<int>(&i);
    const auto cpc = ptr<const int>(&i);

    REQUIRE(p.get() == &i);
    REQUIRE(pc.get() == &i);
    REQUIRE(cp.get() == &i);
    REQUIRE(cpc.get() == &i);

    REQUIRE(i == 42);

    *p = 1;
    REQUIRE(i == 1);

    //*pc = 2; // error

    *cp = 3;
    REQUIRE(i == 3);

    //*cpc = 4; // error

    ptr<const int> pc2;
    pc2 = p;
    REQUIRE(pc2.get() == &i);

    // ptr<int> p2 = pc2; // error

    const ptr<const int> cpc2 = p;
    REQUIRE(cpc2.get() == &i);
}
