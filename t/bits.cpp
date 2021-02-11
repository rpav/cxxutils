#include <cstdint>

#include <catch2/catch.hpp>
#include <rpav/bits.hpp>
#include <rpav/bitset.hpp>
#include <rpav/log.hpp>

using namespace rpav;

TEST_CASE("bits/simple", "[bits]")
{
    REQUIRE(ctz(~uint8_t{0}) == 0);
    REQUIRE(ctz(~uint16_t{0}) == 0);
    REQUIRE(ctz(~uint32_t{0}) == 0);
    REQUIRE(ctz(~uint64_t{0}) == 0);

    REQUIRE(ctz(uint8_t{1}) == 0);

    REQUIRE(ctz(uint64_t{0x1ULL}) == 0);
    REQUIRE(ctz(uint64_t{0x0ULL}) == 64);

    {
        const uint64_t n64 = 0x1FFFFFFC00000000;
        const uint32_t n32 = 0x1FFFFFFC;

        REQUIRE(bsr(n64) == 60);
        REQUIRE(clz(n64) == 3);
        REQUIRE(ctz(n64) == 34);

        REQUIRE(bsr(uint64_t{0xFFFFFFFF00000000LL}) == 63);
        REQUIRE(clz(uint64_t{1}) == 63);
        REQUIRE(clz(uint64_t{0}) == 64);

        REQUIRE(clz(uint32_t{0}) == 32);

        REQUIRE(bsr(n32) == 28);
        REQUIRE(clz(n32) == 3);
        REQUIRE(ctz(n32) == 2);
    }

    {
        const uint64_t n64 = 0x0;
        const uint32_t n32 = 0x0;
        const uint8_t  n8  = 0x0;

        REQUIRE(clz(n8) == 8);
        REQUIRE(clz(n32) == 32);
        REQUIRE(clz(n64) == 64);

        REQUIRE(ctz(n8) == 8);
        REQUIRE(ctz(n32) == 32);
        REQUIRE(ctz(n64) == 64);
    }
}

TEST_CASE("bitset/count", "[bits][bitset]")
{
    {
        bitset32<32> bs32 = 0xFFFFFFFF;

        REQUIRE(bs32.clz() == 0);
        REQUIRE(bs32.ctz() == 0);

        bitset64<64> bs64 = 0xFFFFFFFFFFFFFFFFULL;

        REQUIRE(bs64.clz() == 0);
        REQUIRE(bs64.ctz() == 0);
    }

    {
        bitset8<8>   bs8;
        bitset32<32> bs32;
        bitset64<64> bs64;

        REQUIRE(bs8.clz() == 8);
        REQUIRE(bs8.ctz() == 8);

        REQUIRE(bs32.clz() == 32);
        REQUIRE(bs32.ctz() == 32);

        REQUIRE(bs64.clz() == 64);
        REQUIRE(bs64.ctz() == 64);
    }

    {
        bitset8<16>   bs8;
        bitset32<64>  bs32;
        bitset64<128> bs64;

        REQUIRE(bs8.clz() == 16);
        REQUIRE(bs8.ctz() == 16);

        REQUIRE(bs32.clz() == 64);
        REQUIRE(bs32.ctz() == 64);

        REQUIRE(bs64.clz() == 128);
        REQUIRE(bs64.ctz() == 128);
    }
}

TEST_CASE("bitset/typecheck", "[bits][bitset][type]")
{
    REQUIRE(std::is_same_v<bitset_for<64>::type, bitset64<64>>);
    REQUIRE(std::is_same_v<bitset_for<32>::type, bitset32<32>>);
    REQUIRE(std::is_same_v<bitset_for<16>::type, bitset16<16>>);
    REQUIRE(std::is_same_v<bitset_for<8>::type, bitset8<8>>);
}

TEST_CASE("bitset/maskedcount", "[bits][bitset]") {
    bitset8<8> bs8 = 0x1;
    bitset16<16> bs16 = 0x1;
    bitset32<32> bs32 = 0x1;

    REQUIRE(bs8.ctz() == 0);
    REQUIRE(bs8.ctz(1) == 8);

    REQUIRE(bs16.ctz() == 0);
    REQUIRE(bs16.ctz(1) == 16);

    REQUIRE(bs32.ctz() == 0);
    REQUIRE(bs32.ctz(1) == 32);
}
