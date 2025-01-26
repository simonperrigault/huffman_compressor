#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <math.h>
#include <bitset>
#include <sstream>

#include "Compressor.h"

bool operator<(const Node l, const Node r) {
    return l.c < r.c;
}

void inorder_node(const Node* curr, int depth) {
    if (curr == nullptr) return;
    if (curr->right != nullptr) {
        inorder_node(curr->right, depth+1);
        for (int i = 0; i < depth; i++) {
            std::cout << "  ";
        }
        std::cout << " /\n";
    }
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << curr->c << "\n";
    if (curr->left != nullptr) {
        for (int i = 0; i < depth; i++) {
            std::cout << "  ";
        }
        std::cout << " \\\n";
        inorder_node(curr->left, depth+1);
    }
}

int tree_depth(const Node* curr, int depth) {
    if (curr == nullptr) return depth-1;
    return std::max(tree_depth(curr->left, depth+1), tree_depth(curr->right, depth+1));
}

void fill_flat_tree(const Node* curr, int i, std::vector<uint8_t>& flat_tree, std::array<std::string, 256>& char_to_code, std::string&& curr_char) {
    if (curr == nullptr) return;
    flat_tree[i] = curr->c;
    char_to_code[curr->c] = curr_char;
    std::cout << i <<"\n";
    fill_flat_tree(curr->left, 2*i+1, flat_tree, char_to_code, curr_char+"0");
    fill_flat_tree(curr->right, 2*i+2, flat_tree, char_to_code, curr_char+"1");
}
void fill_char_to_code(const Node* curr, std::array<std::string, 256>& char_to_code, std::string&& curr_char) {
    if (curr == nullptr) return;
    char_to_code[curr->c] = curr_char;
    fill_char_to_code(curr->left, char_to_code, curr_char+"0");
    fill_char_to_code(curr->right, char_to_code, curr_char+"1");
}

void encode_tree(const Node* curr, std::string& output) {
    if (curr == nullptr) return;
    if (curr->left == nullptr && curr->right == nullptr) {
        output += '1';
        output += curr->c;
        return;
    }
    output += '0';
    encode_tree(curr->left, output);
    encode_tree(curr->right, output);
}

void compress(const char* input_filename, const char* output_filename) {
    std::ifstream input_file(input_filename, std::ios_base::binary);
    std::ofstream output_file(output_filename, std::ios_base::binary);

    char curr;
    std::unordered_map<char, int> counter;
    while (input_file) {
        curr = input_file.get();
        counter[curr]++;
    }

    std::priority_queue<std::pair<int, Node*>> heap;
    for (std::pair<char, int> key_val : counter) {
        heap.push({-key_val.second, new Node(key_val.first)});
    }

    while (heap.size() > 1) {
        std::pair<int, Node*> temp(heap.top());
        heap.pop();
        Node* next = new Node(0, temp.second, heap.top().second);
        heap.push({temp.first+heap.top().first, next});
        heap.pop();
    }

    std::array<std::string, 256> char_to_code;
    fill_char_to_code(heap.top().second, char_to_code, "");

    for (int i = 0; i < 256; i++) {
        std::cout << std::bitset<8>(i) << " : " << char_to_code[i] << "\n";
    }

    std::string encoded_tree;
    encode_tree(heap.top().second, encoded_tree);
    uint32_t tree_size = encoded_tree.size();
    for (int i = 0; i < 4; i++) {
        output_file.put((tree_size >> (3-i)*8) & 0xFF);
    }
    output_file << encoded_tree;

    uint8_t buffer = 0;
    int i_buffer = 0;
    input_file.clear();
    input_file.seekg(0);

    while (input_file) {
        curr = input_file.get();
        for (char c : char_to_code[(uint8_t)curr]) {
            buffer <<= 1;
            if (c == '1') {
                buffer ^= 1;
            }
            i_buffer++;
            if (i_buffer == 8) {
                output_file.put(buffer);
                buffer = 0;
                i_buffer = 0;
            }
        }
    }
    output_file.put(buffer << (8-i_buffer)); // we put the last byte with some useless bits
}

void decode_tree(std::string&& curr, std::stringstream& ss, std::unordered_map<std::string, char>& code_to_char) {
    if (!ss) return;

    char c = ss.get();
    if (c == '1') {
        c = ss.get();
        code_to_char[curr] = c;
        return;
    }
    else if (c == '0') {
        decode_tree(curr+'0', ss, code_to_char);
        decode_tree(curr+'1', ss, code_to_char);
    }
}

void decompress(const char* input_filename, const char* output_filename) {
    std::ifstream input_file(input_filename, std::ios_base::binary);
    std::ofstream output_file(output_filename, std::ios_base::binary);

    uint32_t tree_size = 0;
    char curr;
    for (int i = 0; i < 4; i++) {
        curr = input_file.get();
        tree_size <<= 8;
        tree_size ^= curr;
    }

    std::string tree(tree_size, '\0');
    input_file.read(&tree[0], tree_size);
    std::unordered_map<std::string, char> code_to_char;
    std::stringstream ss(tree);
    decode_tree("", ss, code_to_char);

    for (auto kv : code_to_char) {
        std::cout << std::bitset<8>(kv.second) << " : " << kv.first << "\n";
    }
    
    std::string buffer;
    while (input_file) {
        curr = input_file.get();
        for (int i = 7; i >= 0; i--) {
            buffer += '0' + (curr >> i & 1);
            if (code_to_char.count(buffer)) {
                output_file << code_to_char[buffer];
                if (code_to_char[buffer] == EOF) {
                    input_file.setstate(std::ios_base::eofbit);
                    break;
                }
                buffer.clear();
            }
        }
    }
}