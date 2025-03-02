#include <rpav/iterator.hpp>
#include <rpav/str/std.hpp>
#include <rpav/log.hpp>
#include <catch2.hpp>

using namespace rpav;
using namespace rpav::iter;

using ivector = std::vector<int>;

TEST_CASE("with_iterator", "[iterator]") {
    ivector v = { 1, 2, 3, 4 };

    auto c = with_iterator(v);
    auto begin = c.begin();
    auto end = c.end();

    REQUIRE(*begin == v.begin());
    REQUIRE(*end == v.end());
}

TEST_CASE("zip1", "[iterator]") {
    ivector v0 = { 10, 20, 30 };
    ivector v1 = { 100, 200, 300, 400 };

    auto za = zip(v0, v1);

    auto zi = za.begin();
    auto ze = za.end();

    auto [i,j] = *zi;

    REQUIRE(zi != ze);
    REQUIRE(i == 10);
    REQUIRE(j == 100);

    ++zi;
    REQUIRE(zi != ze);

    ++zi;
    REQUIRE(zi != ze);

    ++zi;
    REQUIRE(!(zi != ze));
    REQUIRE(zi == ze);
}

TEST_CASE("zip2", "[iterator]") {
    ivector v = { 10, 20, 30, 40 };
    const char c[] = { 'a', 'b', 'c', 'd', 'e' };
    const int arr[] = { 1, 2, 3, 4 };

    auto za = zip(from(0), reversing(v), c);
    auto zi = za.begin();

    {
        auto [i, j, ch] = *zi;
        REQUIRE((i == 0 && j == 40 && ch == 'a'));
    }

    ++zi; {
        auto [i, j, ch] = *zi;
        REQUIRE((i == 1 && j == 30 && ch == 'b'));
        j = i;
    }

    REQUIRE(v[2] == 1);
}

TEST_CASE("from1", "[iterator]") {
    auto fa = from(0);
    auto f = fa.begin();
    auto fe = fa.end();

    REQUIRE(*f == 0);
    ++f; REQUIRE(*f == 1);
    ++f; REQUIRE(*f == 2);
    REQUIRE(f != fe);

    auto fa1 = from(1, 2);
    auto f1 = fa1.begin();
    auto fe1 = fa1.end();

    REQUIRE(*f1 == 1);
    ++f1; REQUIRE(*f1 == 3);
    ++f1; REQUIRE(*f1 == 5);
    REQUIRE(f1 != fe1);


    auto fa2 = from_to(0, 5, 2);
    auto f2 = fa2.begin();
    auto fe2 = fa2.end();

    REQUIRE(*f2 == 0);
    ++f2; REQUIRE(*f2 == 2);
    ++f2; REQUIRE(*f2 == 4);
    ++f2; REQUIRE(*f2 == 6);
    REQUIRE(f2 == fe2);
}

TEST_CASE("from2", "[iterator]") {
    auto fa = from_to(0.0, 0.3, 0.2);
    auto f = fa.begin();
    auto fe = fa.end();

    REQUIRE(*f == 0.0);
    ++f; REQUIRE(*f == 0.2);
    ++f; REQUIRE(*f == 0.4);
    REQUIRE(f == fe);
}


TEST_CASE("is_last", "[iterator]") {
    ivector v = { 1, 2, 3 };

    auto isl = is_last(v);
    auto it = isl.begin();

    REQUIRE(*it == false);
    ++it; REQUIRE(*it == false);
    ++it; REQUIRE(*it == true);
}
