#include <catch2/catch.hpp>

#include <rpav/any.hpp>
#include <rpav/any_ptr.hpp>

using namespace rpav;

static auto a_lambda = [](bool) -> int { return 0; };

int actual_function(bool b) { return 1; }

class FunClass {
public:
    int operator()(bool b) { return 2; }
};


TEST_CASE("any_fun", "[any]") {
    FunClass fc;
    std::function<int(bool)> fun = actual_function;

    auto f0 = make_function(a_lambda);
    auto f1 = make_function(actual_function);
    auto f2 = make_function(fc);
    auto f3 = make_function(fun);

    REQUIRE(f0);
    REQUIRE(f1);
    REQUIRE(f2);
    REQUIRE(f3);

    any_fun af0 = a_lambda;
    any_fun af1 = actual_function;
    any_fun af2 = fc;
    any_fun af3 = fun;

    REQUIRE(af0);
    REQUIRE(af0.get_if<int(bool)>());
    REQUIRE(af1);
    REQUIRE(af1.get_if<int(bool)>());
    REQUIRE(af2);
    REQUIRE(af2.get_if<int(bool)>());
    REQUIRE(af3);
    REQUIRE(af3.get_if<int(bool)>());

    auto rf0 = *af0.get_if<int(bool)>();
    auto rf1 = *af1.get_if<int(bool)>();
    auto rf2 = *af2.get_if<int(bool)>();
    auto rf3 = *af3.get_if<int(bool)>();

    REQUIRE(rf0(true) == 0);
    REQUIRE(rf1(true) == 1);
    REQUIRE(rf2(true) == 2);
    REQUIRE(rf3(true) == 1);
}
