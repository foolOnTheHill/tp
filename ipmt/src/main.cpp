#include "search.h"

using namespace std;

/* Handles wildcards on the file name. */
vector<string> getFilesName(const char* f) {
	vector<string> files;

	string wildcard(f);

	size_t found = wildcard.find_last_of("/\\");
	bool has_path = !(found == string::npos);
	string path = has_path ? wildcard.substr(0, found + 1) : "./";
	string file = has_path ? wildcard.substr(found + 1) : wildcard;

	found = file.find_last_of('*');
	bool has_wildcard = !(found == string::npos);

	if (!has_wildcard) {
		ifstream ok(wildcard);

		if (ok.good()) {
			files.push_back(wildcard);
		} else {
			printf("Invalid file: '%s'.\n", f);
		}

		return files;
	}

	file = file.substr(0, found);

	DIR *dp = opendir(path.c_str());
	struct dirent *ep;

	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			string match(ep->d_name);
			if (match.find(file) != string::npos) {
				files.push_back(match);
			}
		}
		closedir(dp);
	} else {
		printf("Invalid path: '%s'.\n", path.c_str());
	}

	return files;
}

void show_help() {
  printf("Indexed Pattern Matching Tool - ipmt\n\n");
	printf("Usage:\n");
  printf("\tipmt index textfile\n");
  printf("\tipmt [options] [pattern] textfile [textfile...]\n\n");
  printf("Options:\n");
	printf("\t-h, --help\t\tShows this\n");
  printf("\t-a, --array\tIndexes the file using a Suffix Array\n");
	printf("\t-p, --pattern file\tMakes the matching using each line in 'file' as a pattern\n");
	printf("\t-c, --count\tOnly prints how many times the patterns matches\n");
	exit(0);
}

int main(int argc, char** argv) {

  return 0;
}
