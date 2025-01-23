#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

#include "Compressor.h"

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

void compress(const char* input_filename, const char* output_filename) {
    std::fstream input_file(input_filename, std::ios::in | std::ios::binary);
    char curr;
    std::unordered_map<char, int> counter;
    while (input_file.get(curr)) counter[curr]++;

    std::priority_queue<std::pair<int, Node*>> heap;
    for (std::pair<char, int> key_val : counter) {
        heap.push({-key_val.second, new Node(key_val.first)});
        for (int i = 7; i >= 0; i--) {
            std::cout << ((key_val.first >> i) & 1);
        }
        std::cout << " : " << key_val.second << "\n";
    }
    std::cout << "Heap : \n";
    int symbols_num = heap.size();
    while (heap.size() > 1) {
        std::pair<int, Node*> temp(heap.top());
        heap.pop();
        Node* next = new Node('x', temp.second, heap.top().second);
        heap.push({temp.first+heap.top().first, next});
        heap.pop();
    }
    const Node* curr_node = heap.top().second;
    inorder_node(curr_node, 0);
}

void decompress(const char* input_filename, const char* output_filename) {

}