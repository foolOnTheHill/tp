#ifndef _IPMT_SEARCH
#define _IPMT_SEARCH

#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>

#include "compress.h"
#include "SuffixTree.cpp"
#include "SuffixArray.cpp"

#include <dirent.h>

using namespace std;

#define ALPHABET_SIZE 256

void generateIndexTree(string &textfile, const string compressAlg);
void generateIndexArray(string &textfile, const string compressAlg);
void match(vector<string> &patterns, string &textfile, bool onlyCount);

#endif
