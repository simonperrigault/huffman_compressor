#include <iostream>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Wrong number of argument\n";
        std::cerr << "Usage : ./compressor compress|decompress <file>\n";
        return 1;
    }
    std::cout << argv[0] << "\n";
    std::cout << argv[1] << "\n";
    std::cout << argv[2] << "\n";
}