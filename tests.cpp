#include <catch2/catch_test_macros.hpp>
#include "Compressor.h"

TEST_CASE( "Tree encoding" ) {
    std::string res;
    encode_tree(new Node(0, new Node('a'), new Node('b')), res);
    REQUIRE(res == "01a1b");
    res.clear();
    encode_tree(new Node(0, new Node(0, new Node('r'), new Node('t')), new Node('b')), res);
    REQUIRE(res == "001r1t1b");
}