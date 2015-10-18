#ifndef _BOYER_MOORE
#define _BOYER_MOORE

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>

#define ALPHABET_LENGTH 256

using namespace std;

vector<int> match_boyer_moore(string text, string pattern);

#endif
