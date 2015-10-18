#include "sellers.h"

int fi(char x, char y) {
	if (x == y) {
		return 0;
	} else {
		return 1;
	}
}


vector<int> sellers(string pattern, string text, int r) {
	int m = pattern.length();
	int n = text.length();
	int S[m+1];
	int S1[n+1];

	vector<int> occ;

	for (int i = 0; i < n+1; i++) {
		S1[i] = i;
		S[i] = 0;
	}

	for (int j = 1; j < n+1; j++) {
		S[0] = 0;
		for (int i = 1; i < m+1; i++) {
			S[i] = min(min(S1[i-1] + fi(pattern[i-1],text[j-1]),
							S1[i] + 1),
							S[i-1] + 1);

		}

		if (S[m] <= r) {
			occ.push_back(j);

		}

		for (int i = 0; i < m+1; i++) {
			S1[i] = S[i];

		}

	}

	return occ;
}
