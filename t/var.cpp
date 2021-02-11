#include <catch2/catch.hpp>
#include <rpav/dubious/var.hpp>

using namespace rpav;

class Foo {
public:
    int bar() { return 1; }
};

TEST_CASE("Value", "[var]") {
    var x = 1;

    x++;

    REQUIRE(x == 2);
}

TEST_CASE("Lambda returning void", "[var]") {
    int i = 1;
    var x = [](int &j) -> void { j++; };

    x(i);

    REQUIRE(i == 2);
}

TEST_CASE("Lambda returning reference", "[var]") {
    int i = 1;
    var x = [](int &j) -> int & { return j; };

    auto &&k = x(i);
    k = 20;

    REQUIRE(i == 20);
}

TEST_CASE("Reference", "[var]") {
    int i = 1;
    var x = i;

    x++;

    REQUIRE(i == 2);
}

TEST_CASE("Pointer", "[var]") {
    int i = 1;
    const int j = 2;

    var x = &i;
    var y = &j;

    (*x)++;

    // error:
    //(*y)++;

    REQUIRE(i == 2);
    REQUIRE(*y == j);

    Foo foo;
    var f = &foo;

    REQUIRE((*f).bar() == 1);
    REQUIRE(f->bar() == 1);
}
