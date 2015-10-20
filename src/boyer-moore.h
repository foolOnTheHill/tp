#ifndef _BOYER_MOORE
#define _BOYER_MOORE

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

#define ALPHABET_LENGTH 256

using namespace std;

void prepareBoyerMoore(const string pattern, int* &occ, int* &f, int* &s);

vector<int> matchBoyerMoore(const string text, const string pattern, int* occ, int* f, int* s);

#endif
