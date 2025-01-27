# Huffman Compressor and Decompressor

Simple implementation of Huffman compressor and decompressor in C++. Tested on text files only. 

## How to use

Compile with CMake

To compress/decompress a file, run :  
```build/compressor compress|decompress <input_file> <output_file>```

To compare 2 files, run :  
```build/compressor compare <file1> <file2>```

## Results

I obtained almost 50% compression rate on all the files I tested, with the biggest file being a 100MB text file.