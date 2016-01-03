#ifndef _IPMT_LZ77
#define _IPMT_LZ77

#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <tuple>
#include <sstream>

#define ALPHABET_SIZE 256
#define WINDOW_SIZE 40
#define BUFFER_SIZE 20

using namespace std;

void compress_lz77(const string &text, string &ret);
void decompress_lz77(string &comp_text, string &ret);

#endif
