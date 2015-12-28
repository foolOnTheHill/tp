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

void show_usage() {
  printf("Usage:\n");
  printf("\tipmt index [options] textfile\n");
  printf("\tipmt search [options] [pattern] textfile [textfile...]\n\n");
}

void show_help() {
  printf("Indexed Pattern Matching Tool - ipmt\n\n");
  show_usage();
  printf("Options:\n");
	printf("\t-h, --help\tShows this\n");
  printf("\t-a, --array\tIndexes the file using a Suffix Array\n");
	printf("\t-p, --pattern \tMakes the matching using each line in 'file' as a pattern\n");
	printf("\t-c, --count\tOnly prints how many times the patterns matches\n");
	exit(0);
}

int main(int argc, char** argv) {
  bool multi_pattern = false;
  bool only_counting = false;
  bool index;
  bool array = false;

  string patterns_file;
  vector<string> patterns;
  vector<string> text_files;

  static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "pattern", required_argument, 0, 'p' },
		{ "count", no_argument, 0, 'c'},
    { "array", no_argument, 0, 'a'},
    { 0, 0, 0, 0 }
	};

	if (argc < 1) {
		show_help();
	}

  if (strcmp(argv[1], "search") == 0) {
		// printf("Search mode\n");
    index = false;
  } else if (strcmp(argv[1], "index") == 0) {
		// printf("Index mode\n");
		index = true;
  } else {
    show_help();
  }

  int option;
  int option_index = 0;
  int arg_index = 2;

  while ((option = getopt_long(argc, argv, "hp:c:a", long_options, &option_index)) != -1) {
    switch(option) {
    case 'h':
      show_help();
      break;
    case 'p':
      if (index) {
        show_usage();
      }
      multi_pattern = true;
      if (optarg == NULL) {
        show_usage();
      }
      patterns_file = optarg;
      arg_index += 2;
      break;
    case 'c':
      if (index) {
        show_usage();
      }
      only_counting = true;
      arg_index += 1;
      break;
    case 'a':
      array = true;
      arg_index += 1;
      break;
    default:
      show_help();
      break;
    }
  }


  if (arg_index >= argc) {
    show_usage();
  }

  if (index) {
		// printf("File to index %s\n", argv[arg_index]);
    string input_file(argv[arg_index]);
    if (array) {
			// printf("Using suffix array.\n");
      generateIndexArray(input_file);
    } else {
			// printf("Using suffix tree.\n");
      generateIndexTree(input_file);
    }
  } else {
    if (!multi_pattern) {
			// printf("Single pattern %s\n", argv[arg_index]);
			patterns.push_back(argv[arg_index]);
      arg_index += 1;
    } else {
      ifstream patterns_stream(patterns_file);

      if (!patterns_stream.good()) {
        printf("Invalid file: '%s'.\n", patterns_file.c_str());
      }

      for(string text; getline(patterns_stream, text); ) {
        patterns.push_back(text);
      }
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
      show_usage();
    }

    for (string &tf : text_files) {
			// printf("Searching on text file %s\n", tf.c_str());
      match(patterns, tf, only_counting);
    }
  }

  return 0;
}
