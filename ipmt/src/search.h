#ifndef _IPMT_SEARCH
#define _IPMT_SEARCH

#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>

#include "lzw.h"
#include "SuffixTree.cpp"
#include "SuffixArray.cpp"

#include <dirent.h>

using namespace std;

#define ALPHABET_SIZE 256

void generateIndexTree(string &textfile);
void generateIndexArray(string &textfile);
void match(vector<string> &patterns, string &textfile, bool onlyCount);

#endif
