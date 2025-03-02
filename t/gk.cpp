#include <catch2.hpp>

#include <gk/core.hpp>

TEST_CASE("gk_rect_overlap", "[gk][gk_rect]") {
    gk::rect r{0,0,2,2};

    // should overlap
    gk::rect o0{1,1,2,2};
    gk::rect o1{1,-1,2,2};
    gk::rect o2{-1,-1,2,2};
    gk::rect o3{-1,1,2,2};

    // should not
    gk::rect n0{2,0,2,2};
    gk::rect n1{0,2,2,2};
    gk::rect n2{0,-2,2,2};
    gk::rect n3{-2,0,2,2};

    gk::rect n4{2,2,2,2};
    gk::rect n5{2,-2,2,2};
    gk::rect n6{-2,-2,2,2};
    gk::rect n7{-2,2,2,2};


    REQUIRE(r.overlap(o0));
    REQUIRE(r.overlap(o1));
    REQUIRE(r.overlap(o2));
    REQUIRE(r.overlap(o3));

    REQUIRE(!r.overlap(n0));
    REQUIRE(!r.overlap(n1));
    REQUIRE(!r.overlap(n2));
    REQUIRE(!r.overlap(n3));
    REQUIRE(!r.overlap(n4));
    REQUIRE(!r.overlap(n5));
    REQUIRE(!r.overlap(n6));
    REQUIRE(!r.overlap(n7));
}
