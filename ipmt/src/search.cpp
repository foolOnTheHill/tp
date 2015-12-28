#include "search.h"

void read(string &filename, string &input) {
  // printf("Reading from file\n");

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

  // printf("Finished reading.\n");
}

void output(string &filename, string &output) {
  ofstream ofs;
  ofs.open(filename.c_str(), ofstream::out | ofstream::trunc);
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

void getIndexFileName(string &textfile, string &indexfile) {
  int id = textfile.rfind(".");
  indexfile = textfile.substr(0, id);
  indexfile += ".idx";
}

void generateIndexTree(string &textfile) {
  string input, treeRepr, indexRepr, encoding, indexFileName;

  read(textfile, input);

  // printf("Creating tree.\n");
  SuffixTree tree(input);
  // printf("Getting representation.\n");
  tree.getRepr(treeRepr);

  // printf("Preparing bytes\n");
  ostringstream os;
  os << input.size() << '\n';
  os << treeRepr.size() << '\n';
  os << input;
  os << treeRepr;
  indexRepr = os.str();

  // printf("Compressing.\n");
  compress(indexRepr, encoding);

  getIndexFileName(textfile, indexFileName);

  // printf("Saving.\n");
  output(indexFileName, encoding);
  printf("Created suffix tree index file '%s' for input file '%s'.\n", indexFileName.c_str(), textfile.c_str());
  exit(0);
}

void generateIndexArray(string &textfile) {
  string input, arrayRepr, indexRepr, encoding, indexFileName;

  read(textfile, input);

  // printf("Preparing encoding\n");

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
  // printf("Printing matches.\n");
  // printf("%lu\n", occ.size());
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

void match(vector<string> &patterns, string &textfile, bool onlyCount) {
  string encoding, input, text, repr;
  char structure;

  // printf("Reading index file.\n");
  read(textfile, encoding);
  // printf("Decompressing.\n");
  decompress(encoding, input);
  // printf("Recovering original data.\n");
  recoverFromIndex(input, text, repr, structure);

  if (structure == 'A') {
    // printf("Using a suffix array.\n");
    SuffixArray array(text, repr);
    for (string &p : patterns) {
      vector<int> occ = array.match(p);
      printOccurences(textfile, p, occ, onlyCount);
    }
  } else {
    // printf("Using a suffix tree.\n");
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
