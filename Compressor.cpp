#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <math.h>
#include <bitset>

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
    std::fstream input_file(input_filename, std::ios::in | std::ios::binary);
    std::fstream output_file(output_filename, std::ios::out | std::ios::binary);

    uint8_t curr;
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

    inorder_node(heap.top().second, 1);

    std::array<std::string, 256> char_to_code;
    fill_char_to_code(heap.top().second, char_to_code, "");

    std::string encoded_tree;
    encode_tree(heap.top().second, encoded_tree);
    std::cout << encoded_tree << "\n";
}

void decompress(const char* input_filename, const char* output_filename) {
}