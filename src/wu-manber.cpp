#include "wu-manber.h"

void char_mask(string pattern, long long int *C) {
	int m = pattern.length();

	int max = 0;
	for (int i = 0; i < m; i++) {
		max |= 1 << i;
	}

	for (int i = 0; i < ALPHABET_LENGTH; i++) {
		C[i] = max;
	}

	long long pos_mask = ~1;

	for (int i = 0; i < m; i++) {
		C[(int) pattern[i]] = C[(int) pattern[i]] & pos_mask;
		pos_mask = pos_mask << 1 | 1;
	}

}

vector<int> wu_manber(string text, string pattern, int r) {
	int m = pattern.length();
	int n = text.length();
	long long int S[r+1];

	long long int C[ALPHABET_LENGTH];
	char_mask(pattern, C);

	vector<int> occ;

	int count = 0;
	long long int msb = 1 << (m-1);

	int max = 0;
	for (int i = 0; i < m; i++) {
		max |= 1 << i;
	}

	S[0] = max;

	for (int q = 1; q < r+1; q++) {
		S[q] = S[q-1] << 1;
	}

	long long int a,b,c,d,S1,S2;

	for (int j = 1; j < n+1; j++) {
		S1 = S[0];
		S[0] = S[0] << 1 | C[(int)text[j-1]];

		for (int q = 1; q < r+1; q++) {
			S2 = S[q];

			a = S[q] << 1 | C[(int)text[j-1]];
			b = S1 << 1;
			c = S[q-1] << 1;
			d = S1;

			S[q] = a & b & c & d;
			S1 = S2;
		}

		if ((S[r] & msb) == 0) {
			occ.push_back(j);
			count++;

		}
	}

	return occ;

}
