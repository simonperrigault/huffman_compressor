#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include <string>
#include <array>

typedef struct Node {
    uint8_t c;
    const struct Node* left;
    const struct Node* right;

    Node(uint8_t p, const struct Node* l = nullptr, const struct Node* r = nullptr) {
        c = p;
        left = l;
        right = r;
    } 
} Node;
bool operator<(const Node l, const Node r);

void compress(const char* input_filename, const char* output_filename);
void decompress(const char* input_filename, const char* output_filename);

void encode_tree(const Node* curr, std::string& output);
void fill_flat_tree(const Node* curr, int i, std::vector<uint8_t>& flat_tree, std::array<std::string, 256>& char_to_code, std::string&& curr_char);
void fill_char_to_code(const Node* curr, std::array<std::string, 256>& char_to_code, std::string&& curr_char);
int tree_depth(const Node* curr, int depth);
void inorder_node(const Node* curr, int depth);

#endif