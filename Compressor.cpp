#include <fstream>
#include <queue>

#include "Compressor.h"

void fill_char_to_code(std::string&& curr, std::stringstream& ss, std::array<std::string, 256>& char_to_code) {
    if (!ss) return;

    char c = ss.get();
    if (c == '1') {
        c = ss.get();
        char_to_code[(unsigned char)c] = curr;
        return;
    }
    else if (c == '0') {
        fill_char_to_code(curr+'0', ss, char_to_code);
        fill_char_to_code(curr+'1', ss, char_to_code);
    }
}

void compress(const char* input_filename, const char* output_filename) {
    std::ifstream input_file(input_filename, std::ios_base::binary);
    std::ofstream output_file(output_filename, std::ios_base::binary);

    char curr;
    std::array<long long, 256> counter;
    counter.fill(0);
    while (input_file) {
        curr = input_file.get();
        counter[(unsigned char)curr]++;
    }

    std::priority_queue<std::pair<long long, std::string>> heap;
    for (int i = 0; i < 256; i++) {
        if (counter[i] == 0) continue;
        std::string leaf("1");
        leaf.push_back(i);
        heap.push({-counter[i], leaf});
    }

    while (heap.size() > 1) {
        std::pair<int, std::string> temp(heap.top());
        heap.pop();
        heap.push({temp.first+heap.top().first, "0"+temp.second+heap.top().second});
        heap.pop();
    }

    std::string encoded_tree(std::move(heap.top().second));
    uint32_t tree_size = encoded_tree.size();
    for (int i = 0; i < 4; i++) {
        output_file.put((tree_size >> (3-i)*8) & 0xFF);
    }
    output_file << encoded_tree;

    std::array<std::string, 256> char_to_code;
    std::stringstream ss(encoded_tree);
    fill_char_to_code("", ss, char_to_code);

    char buffer = 0;
    int i_buffer = 0;
    input_file.clear();
    input_file.seekg(0);

    while (input_file) {
        curr = input_file.get();
        for (char c : char_to_code[(unsigned char)curr]) {
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
        tree_size ^= (curr & 0xFF);
    }

    std::string tree(tree_size, '\0');
    input_file.read(&tree[0], tree_size);
    std::unordered_map<std::string, char> code_to_char;
    std::stringstream ss(tree);
    decode_tree("", ss, code_to_char);
    
    std::string buffer;
    while (input_file) {
        curr = input_file.get();
        for (int i = 7; i >= 0; i--) {
            buffer += '0' + (curr >> i & 1);
            if (code_to_char.count(buffer)) {
                if (code_to_char[buffer] == EOF) {
                    input_file.setstate(std::ios_base::eofbit);
                    break;
                }
                else {
                    output_file << code_to_char[buffer];
                    buffer.clear();
                }
            }
        }
    }
}

bool compare_files(const char* filename1, const char* filename2) {
    char c1, c2;
    std::ifstream file1(filename1, std::ios_base::binary), file2(filename2, std::ios_base::binary);
    while (file1 && file2) {
        c1 = file1.get();
        c2 = file2.get();
        if (c1 != c2) return false;
    }
    if (file1 || file2) return false;
    return true;
}