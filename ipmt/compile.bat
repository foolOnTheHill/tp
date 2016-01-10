mkdir bin
g++ -std=c++11 -Wall -Wextra -w -O2 src/main.cpp src/search.cpp src/lzw.cpp src/lz77.cpp src/compress.cpp src/SuffixTree.cpp src/SuffixArray.cpp -o bin/pmt
