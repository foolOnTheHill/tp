#ifndef _AHO_CORASICK
#define _AHO_CORASICK

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <queue>

#define ALPHABET_LENGTH 256

using namespace std;

void prepareAhoCorasick(vector<string> patterns, int* &f, int** &g, long long* &out, int totalPatternsLength);

vector<int>* matchAhoCorasick(string text, vector<string> patterns, int* &f, int** &g, long long* &out);

#endif
