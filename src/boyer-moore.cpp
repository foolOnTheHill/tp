#include "boyer-moore.h"

using namespace std;

void prepareBoyerMoore(string pattern, int* &occ, int* &f, int* &s) {
  int m = pattern.length();

  int i = m;
  int j = m+1;
  int c;

  /* Pre-proccessing for Bad Character Heuristics */
  for (int k = 0; k < ALPHABET_LENGTH; k++) {
    occ[k] = -1;
  }

  for (int k = 0; k < m+1; k++) {
    f[k] = 0;
    s[k] = 0;

    /* Pre-proccessing for Bad Character Heuristics */
    if (k < m) {
      c = pattern[k];
      occ[c] = k;
    }
  }

  f[i] = j;

  /* Good Sufix Heuristics Case 1 */
  while (i > 0) {
      while (j <= m && pattern[i-1] != pattern[j-1]) {
        if (s[j] == 0) {
          s[j] = j-i;
        }
        j = f[j];
      }
      i--;
      j--;
      f[i] = j;
  }

  /* Good Sufix Heuristics Case 2 */
  j = f[0];
  for (i = 0; i < m+1; i++) {
    if (s[i] == 0) {
      s[i] = j;
    }
    if (i == j) {
      j = f[j];
    }
  }

}

vector<int> matchBoyerMoore(string text, string pattern, int* occ, int* f, int* s) {
  int m = pattern.length();
  int n = text.length();

  int j;
  int i = 0;

  vector<int> pos;

  while (i <= n-m) {
    j = m-1;

    while (j >= 0 && pattern[j] == text[i+j]) {
      j--;
    }

    if (j < 0) {
      pos.push_back(i);
      i += s[0];
    } else {
      i += max(s[j+1], j - occ[text[i+j]]);
    }
  }

  return pos;
}
