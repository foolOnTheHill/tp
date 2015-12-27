#ifndef _IPMT_SEARCH
#define _IPMT_SEARCH

#include <bits/stdc++.h>
#include "SuffixTree.cpp"
#include "SuffixArray.cpp"

#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>

#define ALPHABET_SIZE 256

using namespace std;

void generateIndexTree(string &textfile);
void generateIndexArray(string &textfile);
void match(vector<string> &patterns, string &textfile);

void compress(const string &text, string &ret);
void decompress(string &comp_text, string &ret);

#endif
