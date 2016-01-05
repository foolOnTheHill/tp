#include "lzw.h"

void compress_lzw(const string &text, string &comp_file) {
  unordered_map<string, int> dict;

  ofstream ofs;
  ofs.open(comp_file.c_str(), ofstream::out | ofstream::trunc);

  ofs << "LZW";

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
      ofs << dict[aux] << '\n';

      dictSize += 1;
      dict[tmp] = dictSize;
      aux = chr;
    }
  }

  if (!aux.empty()) {
    ofs << dict[aux] << '\n';
  }

  ofs.close();
}

void decompress_lzw(string &comp_file, string &ret) {
  ifstream ifs(comp_file.c_str());

  if (!ifs.good()) {
    printf("Invalid pattern file '%s'!\n", comp_file.c_str());
    exit(1);
  }

  char c_aux;
  ifs >> c_aux;
  ifs >> c_aux;
  ifs >> c_aux;

  unordered_map<int, string> dict;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    dict[i] = string(1, i);
  }

  int prev, curr;
  string ipt, aux;

  ifs >> prev;
  ret = dict[prev];
  aux = ret;

  int dictSize = ALPHABET_SIZE;

  while(ifs >> curr) {
    if (dict.count(curr)) {
      ipt = dict[curr];
    } else {
      ipt = aux + aux[0];
    }
    ret += ipt;

    dictSize += 1;
    dict[dictSize] = aux + ipt[0];

    prev = curr;
    aux = ipt;
  }

  ifs.close();
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
