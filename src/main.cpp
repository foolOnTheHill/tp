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
vector<char*> get_files_name(char* f) {
	vector<char*> files;

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
		while ((ep = readdir(dp))) {
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

void help() {
	printf("Uso: pmt [options] [pattern] textfile [textfile...]");
	printf("-h, --help\t\tGuia de ajuda");
	printf("-e, --edit d\t\tFaz uma busca aproximada em uma distância máxima 'd'");
	printf("-p, --pattern file\tUtiliza todos os padrões em 'file' como entrada");

	exit(0);
}

int main(int argc, char **argv) {
	bool multi_pattern = false;
	bool approximate_matching = false;

	int edit_distance = 0;
	string patterns_file;
	vector<char*> pattern_files;
	vector<char*> text_files;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "edit", required_argument, 0, 'e' },
		{ "pattern", required_argument, 0, 'p' },
		{ 0, 0, 0, 0 }
	};

	int option;
	int option_index = 0;
	int options = 0;

	while ((option = getopt_long(argc, argv, "he:p", long_options, &option_index)) != -1) {
		switch (option) {
		case 'e':
			approximate_matching = true;

			edit_distance = atoi(optarg);
			options += 2;

			break;
		case 'p':
			multi_pattern = true;

			patterns_file = optarg;
			options += 2;

			break;
		case 'h':
		case '?':
		default:
			help();

			break;
		}
	}

	if (options >= argc || argc == 1) {
		help();
	}

	if (!multi_pattern) {
		options += 1;

		pattern_files.push_back(argv[options]);
	} else {
		ifstream patterns_stream(patterns_file);

		if (!patterns_stream.good()) {
			printf("Arquivo de padrões %s inválido\n", patterns_file.c_str());
		}

		char* pattern;

		while (patterns_stream >> pattern) {
			pattern_files.push_back(pattern);
		}
	}

	if (argc <= options + 1) {
		printf("Insira um ou mais arquivos de texto para realizar a busca\n");

		help();
	}

	for (int i = options + 1; i < argc; i++) {
		for (char* file_name : get_files_name(argv[i])) {
			pattern_files.push_back(file_name);
		}
	}

	return 0;
}
