#include "lzw.h"

void compress(const string &text, string &ret) {
  vector<int> comp_text;
  map<string, int> dict;

  for (int i = 0; i < ALPHABET_SIZE; i++) {
    dict[string(1, i)] = i;
  }

  string aux = "";
  char chr;

  for (int i = 0; i < text.length(); i++) {
    chr = text[i];

    string tmp = aux + chr;

    if (dict.count(tmp)) {
      aux = tmp;
    } else {
      comp_text.push_back(dict[aux]);
      int s = dict.size() + 1;
      dict[tmp] = s;
      aux = chr;
    }
  }

  if (!aux.empty()) {
    comp_text.push_back(dict[aux]);
  }

  ostringstream os;
	vector<int>::iterator it;
	for (it = comp_text.begin() ; it != comp_text.end(); ++it) {
		os << *it << '\n';
	}
	ret = os.str();
}

void decompress(string &comp_text, string &ret) {
  vector<int> encode;

	int tmp;
	stringstream ss(comp_text);
	while ( ss >> tmp ) {
		encode.push_back(tmp);
	}

  map<int, string> dict;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    dict[i] = string(1, i);
  }

  int prev, curr;
  string ipt, aux;

  prev = encode[0];
  ret = dict[prev];
  aux = ret;

  for (int i = 1, N = encode.size(); i < N; i++) {
    curr = encode[i];

    if (dict.count(curr)) {
      ipt = dict[curr];
    } else {
      ipt = aux + aux[0];
    }
    ret += ipt;

    int s = dict.size() + 1;
    dict[s] = aux + ipt[0];

    prev = curr;
    aux = ipt;
  }
}
