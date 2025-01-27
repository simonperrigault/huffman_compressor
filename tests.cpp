#include <catch2/catch_test_macros.hpp>
#include "Compressor.h"

TEST_CASE("Compare 2 files") {
    REQUIRE(compare_files("data/mini_text.txt", "data/mini_text.txt") == true);
    REQUIRE(compare_files("data/mini_text.txt", "data/text.txt") == false);
}

TEST_CASE("Compress and decompress") {
    compress("data/text.txt", "data/text.zip");
    decompress("data/text.zip", "data/text_dezip.txt");
    REQUIRE(compare_files("data/text.txt", "data/text_dezip.txt") == true);
}