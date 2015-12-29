#include "search.h"

using namespace std;

void read(string &filename, string &input) {

  ifstream ifs(filename.c_str());

  if (!ifs.good()) {
    printf("Invalid pattern file '%s'!\n", filename.c_str());
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
  ofs.open(filename.c_str(), ofstream::out | ofstream::trunc);
  ofs << output;
  ofs.close();
}

void getIndexFileName(string &textfile, string &indexfile) {
  int id = textfile.rfind(".");
  indexfile = textfile.substr(0, id);
  indexfile += ".idx";
}

void generateIndexTree(string &textfile) {
  string input, treeRepr, indexRepr, encoding, indexFileName;

  read(textfile, input);

  SuffixTree tree(input);
  tree.getRepr(treeRepr);

  ostringstream os;
  os << input.size() << '\n';
  os << treeRepr.size() << '\n';
  os << input;
  os << treeRepr;
  indexRepr = os.str();

  compress(indexRepr, encoding);

  getIndexFileName(textfile, indexFileName);

  output(indexFileName, encoding);
  printf("Created suffix tree index file '%s' for input file '%s'.\n", indexFileName.c_str(), textfile.c_str());
  exit(0);
}

void generateIndexArray(string &textfile) {
  string input, arrayRepr, indexRepr, encoding, indexFileName;

  read(textfile, input);

  SuffixArray array(input);

  array.getRepr(arrayRepr);

  ostringstream os;
  os << input.size() << '\n';
  os << (-1) * ((int) arrayRepr.size()) << '\n';
  os << input;
  os << arrayRepr;
  indexRepr = os.str();

  compress(indexRepr, encoding);

  getIndexFileName(textfile, indexFileName);

  output(indexFileName, encoding);
  printf("Created suffix array index file '%s' for input file '%s'.\n", indexFileName.c_str(), textfile.c_str());
  exit(0);
}

void recoverFromIndex(string &input, string &text, string &repr, char &dataStructure) {
  int textSize, size;
  char isArray = false;

  char * buffer = const_cast<char*> (input.c_str() );
  sscanf(buffer, "%d", &textSize);

  char *pch = strchr(buffer, '\n');
  buffer = pch+1;
  sscanf(buffer, "%d", &size);

  if(size < 0){
    size = -size;
    isArray = true;
  }

  pch = strchr(buffer, '\n');
  buffer = pch+1;

  char *content = new char[textSize+1];
  memcpy(content, buffer, textSize);

  content[textSize] = '\0';
  text = content;

  buffer = buffer + textSize;
  char *structure = new char[size+1];
  memcpy(structure, buffer, size);

  structure[size] = '\0';
  repr = structure;

  if (isArray) {
    dataStructure = 'A';
  } else {
    dataStructure = 'T';
  }
}

void printCount(string &textfile, string &pattern, vector<int> occ) {
  printf("%s: %lu occurences for '%s'\n", textfile.c_str(), occ.size(), pattern.c_str());
}

void printMatching(string &textfile, string &pattern, vector<int> occ) {
  for (int j = 0, o = occ.size(); j < o; j++) {
    printf("%s:%d: %s\n", textfile.c_str(), occ[j], pattern.c_str());
  }
}

void printOccurences(string &textfile, string &pattern, vector<int> occ, bool onlyCount) {
  if (onlyCount) {
    printCount(textfile, pattern, occ);
  } else {
    printMatching(textfile, pattern, occ);
  }
}

void getFileExtension(string filename, string &extension) {
  int id = filename.rfind(".");
  extension = filename.substr(id+1, filename.length());
}

void match(vector<string> &patterns, string &textfile, bool onlyCount) {
  string encoding, input, text, repr;
  char structure;

  string extension;
  getFileExtension(textfile, extension);
  if (strcmp(extension.c_str(), "idx") != 0) {
    printf("Invalid index file '%s'.\n", textfile.c_str());
    exit(1);
  }

  read(textfile, encoding);
  decompress(encoding, input);
  recoverFromIndex(input, text, repr, structure);

  if (structure == 'A') {
    SuffixArray array(text, repr);
    for (string &p : patterns) {
      vector<int> occ = array.match(p);
      printOccurences(textfile, p, occ, onlyCount);
    }
  } else {
    SuffixTree tree(text, repr);
    for (string &p : patterns) {
      vector<int> occ = tree.match(p);
      printOccurences(textfile, p, occ, onlyCount);
    }
  }
}

// int main() {
//   string text("All in all it's just another brick in the wall.");
//   string encoded;
//   string recovered;

//   compress(text, encoded);
//   decompress(encoded, recovered);

//   printf("%s\n", text.c_str());
//   printf("%s\n", recovered.c_str());

//   return 0;
// }
