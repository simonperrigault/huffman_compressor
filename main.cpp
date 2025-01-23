#include <iostream>
#include <string.h>

#include "Compressor.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Wrong number of argument\n";
        std::cerr << "Usage : ./compressor compress|decompress <input> <output>\n";
        return 1;
    }
    
    if (!strcmp(argv[1], "compress")) {
        compress(argv[2], argv[3]);
    }
    else if (!strcmp(argv[1], "decompress")) {
        decompress(argv[2], argv[3]);
    }
}