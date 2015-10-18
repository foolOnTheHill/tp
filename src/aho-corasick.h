#ifndef _AHO_CORASICK
#define _AHO_CORASICK

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

#define ALPHABET_LENGTH 256

using namespace std;

void prepareAhoCorasick(vector<const char*> patterns, int* &f, int** &g, long long* &out, int totalPatternsLength);

vector<int>* matchAhoCorasick(const char* text, vector<const char*> patterns, int* &f, int** &g, long long* &out);

#endif
