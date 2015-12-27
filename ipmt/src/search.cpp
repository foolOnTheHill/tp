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

// TODO
void compress(const string &text, string &ret);
void decompress(string &comp_text, string &ret);

void generateIndexTree(string &textfile);
void generateIndexArray(string &textfile);

void match(vector<string> &patterns, string &textfile);
