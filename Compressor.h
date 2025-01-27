#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <unordered_map>

void compress(const char* input_filename, const char* output_filename);
void decompress(const char* input_filename, const char* output_filename);

void decode_tree(std::string&& curr, std::stringstream& ss, std::unordered_map<std::string, char>& code_to_char);
void fill_char_to_code(std::string&& curr, std::stringstream& ss, std::array<std::string, 256>& char_to_code);

bool compare_files(const char*, const char*);

#endif