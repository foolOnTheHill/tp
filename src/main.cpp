#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <getopt.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>

#include "boyer-moore.h"
#include "aho-corasick.h"

using namespace std;

/* Handles wildcards on the file name. */
vector<char*> getFilesNames(char* f) {
  vector<char*> files;

  string wildcard(f);

  size_t found = wildcard.find_last_of("/\\");
  bool has_path = !(found == string::npos);
  string path = has_path ? wildcard.substr(0, found+1) : "./";
  string file = has_path ? wildcard.substr(found+1) : wildcard;

  found = file.find_last_of('*');
  bool has_wildcard = !(found == string::npos);

  if (!has_wildcard) {
    ifstream ok(wildcard);

    if (ok.good()) {
      files.push_back(f);
    } else {
      printf("Invalid file: '%s' .\n", f);
    }

    return files;
  }

  file = file.substr(0, found);

  DIR *dp = opendir(path.c_str());
  struct dirent *ep;

  if (dp != NULL) {
    while ( (ep = readdir(dp)) ) {
      string match(ep->d_name);
      if (match.find(file) != string::npos) {
        files.push_back(ep->d_name);
      }
    }
    closedir(dp);
  } else {
    printf("Invalid path: '%s' .\n", path.c_str());
  }

  return files;
}

int main(int argc, char **argv) {
  printf("Under development.\n");
  return 0;
}
