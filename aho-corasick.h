#ifndef AHO_CORASICK
#define AHO_CORASICK

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

#define ALPHABET_LENGTH 256

using namespace std;

vector<int>* matchAhoCorasick(char* text, vector<char*> patterns, int totalPatternsLength);

#endif
