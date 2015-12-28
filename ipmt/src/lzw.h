#ifndef _IPMT_LZW
#define _IPMT_LZW

#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <sstream>

#define ALPHABET_SIZE 256

using namespace std;

void compress(const string &text, string &ret);
void decompress(string &comp_text, string &ret);

#endif
