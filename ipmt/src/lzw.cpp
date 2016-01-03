#include "lzw.h"

void compress_lzw(const string &text, string &ret) {
  vector<int> comp_text;
  unordered_map<string, int> dict;

  for (int i = 0; i < ALPHABET_SIZE; i++) {
    dict[string(1, i)] = i;
  }

  string aux = "";
  char chr;

  int dictSize = ALPHABET_SIZE;

  for (int i = 0; i < text.length(); i++) {
    chr = text[i];

    string tmp = aux + chr;

    if (dict.count(tmp)) {
      aux = tmp;
    } else {
      comp_text.push_back(dict[aux]);
      // int s = dict.size() + 1;
      dictSize += 1;
      dict[tmp] = dictSize;
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
  ret = "LZW\n"+os.str();
}

void decompress_lzw(string &comp_text, string &ret) {
  vector<int> encode;

  int tmp;
  stringstream ss(comp_text);
  while ( ss >> tmp ) {
    encode.push_back(tmp);
  }

  unordered_map<int, string> dict;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    dict[i] = string(1, i);
  }

  int prev, curr;
  string ipt, aux;

  prev = encode[0];
  ret = dict[prev];
  aux = ret;

  int dictSize = ALPHABET_SIZE;

  for (int i = 1; i < (int) encode.size(); i++) {
    curr = encode[i];

    if (dict.count(curr)) {
      ipt = dict[curr];
    } else {
      ipt = aux + aux[0];
    }
    ret += ipt;

    // int s = dict.size() + 1;
    dictSize += 1;
    dict[dictSize] = aux + ipt[0];

    prev = curr;
    aux = ipt;
  }
}
//
// int main() {
//   string text = "All in all it's just another brick in the wall.";
//   string cmp, dcmp;
//   compress(text, cmp);
//   decompress(cmp, dcmp);
//   printf("%s\n%s\n", text.c_str(), dcmp.c_str());
//   // if (strcmp(text, dcmp) != 0) {
//   //   printf("Wrong comp/decomp.\n");
//   // } else {
//   //   printf("All right!\n");
//   // }
//   return 0;
// }
