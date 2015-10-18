#include "aho-corasick.h"

using namespace std;

void initArrays(int* &f, int** &g, long long* &out, int max_states) {
  g = new int*[max_states]; // Go-to function
  f = new int[max_states]; // Fail function
  out = new long long[max_states]; // Output function as a bitwise mask

  for (int i = 0; i < max_states; i++) {
    out[i] = 0;
    f[i] = -1;
    g[i] = new int[ALPHABET_LENGTH];
    for (int j = 0; j < ALPHABET_LENGTH; j++) {
      g[i][j] = -1;
    }
  }
}

int buildFSM(vector<const char*> patterns, int* &f, int** &g, long long* &out, int max_states) {
  int lowestChar = 0;
  int highestChar = ALPHABET_LENGTH;
  int states = 1; // States counter

  // Initialization
  initArrays(f, g, out, max_states);

  // Go-to function update
  int currentState;
  int c;
  for (unsigned int i = 0; i < patterns.size(); i++) {
    const char* keyword = patterns[i];
    currentState = 0;

    for (unsigned int j = 0; j < strlen(keyword); j++) {
      c = keyword[j] - lowestChar;

      // Creates a new node for a previously unseen character
      if (g[currentState][c] == -1) {
        g[currentState][c] = states++;
      }

      currentState = g[currentState][c]; // Keeps following the transition for current pattern
    }

    out[currentState] |= (((long long) 1) << i); // Updates the bitwise mask
  }

  // Creates transitions for characters that are not present on the patterns
  for (int c = 0; c < ALPHABET_LENGTH; c++) {
    if (g[0][c] == -1) {
      g[0][c] = 0;
    }
  }

  // Construction of the failure function
  queue<int> q; // Used to simulate a recursion
  for (int c = 0; c < highestChar - lowestChar; c++) {
    if (g[0][c] != -1 and g[0][c] != 0) {
      f[g[0][c]] = 0; // f(s) = 0 for all states s of depth 1
      q.push(g[0][c]);
    }
  }

  /* From Aho-Corasick (1975):
     Suppose we know f(s') for all states s' of depth d-1.
     For states s of depth d we can compute f(s) as follows:
     - if g(r, a) for all a, do nothing.
     - otherwise, for all symbol a such that g(r, a) = s, do:
        (a) set state = f(r)
        (b) while g(state, a) == fail, do state = f(state)
        (c) set f(s) =  g(state, a)
  */

  int r;
  int state;
  while (!q.empty()) {
    r = q.front();
    q.pop();

    for (int c = 0; c < highestChar - lowestChar; c++) {
      if (g[r][c] != -1) {
        q.push(g[r][c]);

        state = f[r];

        while (g[state][c] == -1) {
          state = f[state];
        }

        f[g[r][c]] = g[state][c];
        out[g[r][c]] |= out[g[state][c]];
      }
    }
  }

  return states;
}

void prepareAhoCorasick(vector<const char*> patterns, int* &f, int** &g, long long* &out, int totalPatternsLength) {
  buildFSM(patterns, f, g, out, totalPatternsLength+1);
}

int delta(int* f, int** g, int currentState, char c) {
  int next = currentState;
  while (g[next][c] == -1) next = f[next];
  return g[next][c];
}

vector<int>* matchAhoCorasick(const char* text, vector<const char*> patterns, int* &f, int** &g, long long* &out) {
  int numPatterns = patterns.size();
  int n = strlen(text);

  vector<int>* matches = new vector<int>[numPatterns];

  int currentState = 0;
  for (int i = 0; i < n; ++i) {
    currentState = delta(f, g, currentState, text[i]);

    if (out[currentState] != 0) {
      for (int j = 0; j < numPatterns; ++j) {
        // Matches pattern #j
        if (out[currentState] & (1 << j)) {
          matches[j].push_back(i - strlen(patterns[j]) + 1);
        }
      }
    }
   }

   return matches;
}
