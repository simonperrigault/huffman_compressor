#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <unordered_map>

typedef struct Node {
    char c;
    const struct Node* left;
    const struct Node* right;

    Node(char p, const struct Node* l = nullptr, const struct Node* r = nullptr) {
        c = p;
        left = l;
        right = r;
    } 
} Node;
bool operator<(const Node l, const Node r);

void compress(const char* input_filename, const char* output_filename);
void decompress(const char* input_filename, const char* output_filename);

void encode_tree(const Node* curr, std::string& output);
void decode_tree(std::string&& curr, std::stringstream& ss, std::unordered_map<std::string, char>& code_to_char);
void fill_char_to_code(const Node* curr, std::array<std::string, 256>& char_to_code, std::string&& curr_char);

bool compare_files(const char*, const char*);

#endif