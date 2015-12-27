#include "search.h"

void read(string &filename, string &input) {
  ifstream ifs(filename);

  if (!ifs.good()) {
    printf("Invalid pattern file '%s'!", filename);
    exit(1);
  }

  string ln;
  while (getline(ifs, ln)) {
    ln += '\n';
    input += ln;
  }

  input = input.substr(0, input.length()-1);
}

void output(string &filename, string &output) {
  ofstream ofs;
  ofs.open(filename, ofstream::out | ofstream::trunc);
  ofs << output;
  ofs.close();
}

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
      dict[tmp] = dict.size() + 1;
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

// TODO
void decompress(string &comp_text, string &ret);

void generateIndexTree(string &textfile);
void generateIndexArray(string &textfile);

void match(vector<string> &patterns, string &textfile);
