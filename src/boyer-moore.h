#ifndef _BOYER_MOORE
#define _BOYER_MOORE

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

#define ALPHABET_LENGTH 256

using namespace std;

vector<int> matchBoyerMoore(string text, string pattern);

#endif
