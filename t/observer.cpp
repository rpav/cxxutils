#include <catch2.hpp>

#include <rpav/log.hpp>
#include <rpav/observer.hpp>

using namespace rpav;

TEST_CASE("observed_simple", "[observer]") {
    observed<std::string> ostr{"hello"};
    bool flag{false};

    ostr.add(&ostr, [&](auto &s) { flag = true; });
    ostr.get() = "foo";
    ostr.notify();

    REQUIRE(flag == true);
    REQUIRE(*ostr == "foo");
}

TEST_CASE("observable", "[observer]") {
    observable<> obs;
    bool flag{false};

    obs.add(&obs, [&]() { flag = true; });
    obs.modify();
    obs.notify();

    REQUIRE(flag == true);
}
