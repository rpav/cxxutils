#include <catch2/catch.hpp>

#include <rpav/dubious/channel.hpp>

using namespace rpav;

TEST_CASE("channel_basic", "[channel]") {
    Channel<int> ch0{-42};
    Channel<int> ch1{ch0};
    Channel<int> ch2{ch1};

    REQUIRE(*ch0 == -42);
    REQUIRE(*ch1 == -42);

    REQUIRE(ch0.isOwner());
    REQUIRE(!ch1.isOwner());
    REQUIRE(!ch2.isOwner());

    REQUIRE(ch1.direct() == &ch0);
    REQUIRE(ch2.direct() == &ch1);
    REQUIRE(ch2.upstream() == &ch0);

    REQUIRE(*ch2 == -42);
    REQUIRE(ch2.direct() == &ch0);

    ch2 = 10;

    REQUIRE(*ch0 == 10);
    REQUIRE(*ch1 == 10);
    REQUIRE(*ch2 == 10);

    ch2.disconnect();

    REQUIRE(ch2.isOwner());
    REQUIRE(*ch2 == 10);

    ch2 = 20;

    REQUIRE(*ch2 == 20);
    REQUIRE(*ch0 == 10);
    REQUIRE(*ch1 == 10);

    ch2.connect(ch1);

    REQUIRE(ch2.direct() == &ch0);
    REQUIRE(*ch2 == 10);
}