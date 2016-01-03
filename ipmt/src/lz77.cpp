#include "lz77.h"

void compress_lz77(const string &text, string &ret) {
  vector<int> comp_text;

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

    comp_text.push_back(pos);
    comp_text.push_back(size);
    comp_text.push_back(static_cast<int>(literal));
  }

  ostringstream os;
  vector<int>::iterator it;
  for (it = comp_text.begin() ; it != comp_text.end(); ++it) {
    os << *it << '\n';
  }
  ret = "LZ7" + os.str();
}

void decompress_lz77(string &comp_text, string &ret) {
  vector<tuple<int, int, char>> encode;

  int p, s, l;

  stringstream ss(comp_text);
  while ( ss >> p ) {
    ss >> s;
    ss >> l;
    encode.push_back(make_tuple(p, s, static_cast<char>(l)));
  }

  ret = "";

  int pos, size;
  char literal;
  for (int i = 0; i < (int) encode.size(); i++) {
    pos = get<0>(encode[i]);
    size = get<1>(encode[i]);
    literal = get<2>(encode[i]);

    int p = ret.length() - pos;
    ret += ret.substr(p, size) + literal;
  }

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
