#include "lz77.h"

void compress_lz77(const string &text, string &comp_file) {
  ofstream ofs;
  ofs.open(comp_file.c_str(), ofstream::out | ofstream::trunc);

  ofs << "LZ7";

  int len = text.length();
  int i = 0;

  string w, b;
  int start, pos, size, literal;
  while (i < len) {
    start = i - WINDOW_SIZE;
    if (start < 0) {
      start = 0;
    }

    w = text.substr(start, i - start);
    b = text.substr(i, BUFFER_SIZE);

    pos = 0;
    size = 0;
    literal = text[i];

    for (int s = b.length(); s > 0; --s) {
      int p = w.rfind(b.substr(0, s));
      if (p >= 0) {
        literal = ' ';
        if (i + s < len) {
          literal = text[i + s];
        }
        pos = w.length() - p;
        size = s;
        break;
      }
    }

    i += size + 1;

    ofs << pos << '\n';
    ofs << size << '\n';
    ofs << static_cast<int>(literal) << '\n';
  }

  ofs.close();
}

void decompress_lz77(string &comp_file, string &ret) {
  ifstream ifs(comp_file.c_str());

  if (!ifs.good()) {
    printf("Invalid pattern file '%s'!\n", comp_file.c_str());
    exit(1);
  }

  char aux;
  ifs >> aux;
  ifs >> aux;
  ifs >> aux;

  ret = "";

  int pos, size, tmp;
  char literal;
  while(ifs >> pos) {
    ifs >> size;
    ifs >> tmp;

    literal = static_cast<char>(tmp);

    int p = ret.length() - pos;
    ret += ret.substr(p, size) + literal;
  }

  ifs.close();
}

// int main() {
//   string text = "All in all it's just another brick in the wall.";
//   string cmp, dcmp;
//   compress_lz77(text, cmp);
//   cmp = cmp.substr(3, cmp.length());
//   decompress_lz77(cmp, dcmp);
//   printf("%s\n%s\n", text.c_str(), dcmp.c_str());
//   return 0;
// }
