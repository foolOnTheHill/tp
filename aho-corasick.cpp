#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>

#define ALPHABET_LENGTH 256

using namespace std;

/* Auxiliary function. Creates a m x n matrix. */
int** initArray(int m, int n) {
  int** delta = new int*[m];

  for (int i = 0; i < m; i++) {
    delta[i] = new int[n];
  }

  for (int i = 0; i < n; i++) {
    delta[0][i] = 0;
  }

  return delta;
}

int** buildAFD(char* pattern) {
  int m = strlen(pattern);
  int n = ALPHABET_LENGTH;

  int** delta = initArray(m, n);

  int curr, prev;
  for (int j = 0; j < m; j++) {
    curr = pattern[j];
    prev = delta[j][curr];
    delta[j][curr] = j + 1;

    for (int k = 0; k < n && j < m-1; k++) {
      delta[j+1][k] = delta[prev][k];
    }
  }

  return delta;
}

vector<int> match(char* text, char* pattern) {
  int** delta = buildAFD(pattern);
  int s = 0;
  int n = strlen(text);
  int m = strlen(pattern);

  vector<int> occ;

  int c;
  for (int i = 0; i < n; i++) {
    c = text[i];
    s = delta[s][c];
    if (s == m) {
      occ.push_back(i - m + 1);
      s = 0;
    }
  }

  return occ;
}

int main() {
  return 0;
}
