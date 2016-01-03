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

void show_usage(bool shouldExit) {
  printf("Usage:\n");
  printf("\tipmt index [options] textfile\n");
  printf("\tipmt search [options] [pattern] textfile [textfile...]\n\n");

	if (shouldExit) {
		exit(0);
	}
}

void show_help() {
  printf("Indexed Pattern Matching Tool - ipmt\n\n");
  show_usage(false);
  printf("Options:\n");
	printf("\t-h, --help\tShows this\n");
  printf("\t-a, --array\tIndexes the file using a Suffix Array\n");
	printf("\t-p, --pattern \tMakes the matching using each line in 'file' as a pattern\n");
	printf("\t-c, --count\tOnly prints how many times the patterns matches\n");
	printf("\t-compress=lz77 \tUses LZ77 to compress the index file\n");
	printf("\t-compress=lzw \tUses LZW to compress the index file\n");
	exit(0);
}

void check(int a, int b) {
	if (a >= b) {
		show_usage(true);
	}
}

void read_patterns(vector<string> &patterns, string patterns_file) {
	ifstream ifs(patterns_file);

	if (!ifs.good()) {
		printf("Invalid file: '%s'.\n", patterns_file.c_str());
	}

	for (string ln; getline(ifs, ln);) {
		patterns.push_back(ln);
	}
}

int main(int argc, char** argv) {
  bool multi_pattern = false;
  bool only_counting = false;
  bool index;
  bool array = false;

  vector<string> patterns;
  vector<string> text_files;

	string input_file;

	if (argc < 2) {
		show_usage(true);
	}

	if (strcmp(argv[1], "search") == 0) {
		index = false;
	} else if (strcmp(argv[1], "index") == 0) {
		index = true;
	} else if (strcmp(argv[1], "-h") == 0) {
		show_help();
	} else {
		show_usage(true);
	}

	int arg_index = 2;
	bool readCountFlag = false, readPatternsFlag = false, readSinglePattern = false, readArrayFlag = false, readCompressFlag = false;
	string alg = "lzw";
	while (arg_index < argc) {
		if (strcmp(argv[arg_index], "-c") == 0) {
			if (readCountFlag || index) {
				show_usage(true);
			}

			only_counting = true;
			arg_index += 1;
		} else if (strcmp(argv[arg_index], "-p") == 0) {
			if (readPatternsFlag || index) {
				show_usage(true);
			}

			multi_pattern = true;
			arg_index += 1;

			check(arg_index, argc);

			string patterns_file(argv[arg_index]);
			read_patterns(patterns, patterns_file);
			arg_index += 1;

			readPatternsFlag = true;
		} else if (strcmp(argv[arg_index], "-compress=lzw") == 0) {
			if (!index || readCompressFlag) {
				show_usage(true);
			}
			alg = "lzw";
			arg_index += 1;
			readCompressFlag = true;
		} else if (strcmp(argv[arg_index], "-compress=lz77") == 0) {
			if (!index || readCompressFlag) {
				show_usage(true);
			}
			alg = "lz77";
			arg_index += 1;
			readCompressFlag = true;
		} else if (strcmp(argv[arg_index], "-a") == 0) {
			if (!index || readArrayFlag) {
				show_usage(true);
			}
			readArrayFlag = true;
			array = true;
			arg_index += 1;
		} else {
			if (index) {
				input_file = argv[arg_index];
			} else if (!readPatternsFlag) {
				string single_pattern(argv[arg_index]);
				patterns.push_back(single_pattern);
				readSinglePattern = true;
				arg_index += 1;
			}
			break;
		}
	}

	if (arg_index > argc) {
		show_usage(true);
	}

	if(index) {
		if (array) {
			generateIndexArray(input_file, alg);
		} else {
			generateIndexTree(input_file, alg);
		}
	} else {

		bool hasPattern = readPatternsFlag || readSinglePattern;
		if (!hasPattern) {
			show_usage(true);
		}

		int qnt_files = 0, tmp;
		for (int i = arg_index; i < argc; i++) {
			vector<string> files_name = getFilesName(argv[i]);

			tmp = files_name.size();

			for (unsigned int i = 0; i < tmp; i++) {
				text_files.push_back(files_name.at(i));
			}

			qnt_files += tmp;
		}

		if (qnt_files == 0) {
			show_usage(true);
		} else {
			for (string &file : text_files) {
				match(patterns, file, only_counting);
			}
		}
	}

  return 0;
}
