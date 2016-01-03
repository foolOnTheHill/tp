#ifndef _IPMT_LZW
#define _IPMT_LZW

#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <sstream>

#define ALPHABET_SIZE 256

using namespace std;

void compress_lzw(const string &text, string &ret);
void decompress_lzw(string &comp_text, string &ret);

#endif
