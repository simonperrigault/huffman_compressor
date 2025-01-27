#include <catch2/catch_test_macros.hpp>
#include "Compressor.h"

TEST_CASE("Compare 2 files") {
    REQUIRE(compare_files("test/mini_text.txt", "test/mini_text.txt") == true);
    REQUIRE(compare_files("test/mini_text.txt", "test/text.txt") == false);
}

TEST_CASE("Compress and decompress") {
    compress("test/mini_text.txt", "test/mini_text.zip");
    decompress("test/mini_text.zip", "test/mini_text_dezip.txt");
    REQUIRE(compare_files("test/mini_text.txt", "test/mini_text_dezip.txt") == true);
    compress("test/text.txt", "test/text.zip");
    decompress("test/text.zip", "test/text_dezip.txt");
    REQUIRE(compare_files("test/text.txt", "test/text_dezip.txt") == true);
}