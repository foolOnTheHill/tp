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
vector<char*> getFilesName(char* f) {
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
				files.push_back(ep->d_name);
			}
		}
		closedir(dp);
	} else {
		printf("Invalid path: '%s'.\n", path.c_str());
	}

	return files;
}

void match_boyer_moore(vector<const char*> text_files, const char* pattern) {
	long long line = 1LL;

	for (unsigned int i = 0; i < text_files.size(); i++) {
		string text_file(text_files.at(i));

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Arquivo inválido: '%s'.\n", text_files[i]);
		}

		for(string text; getline(text_stream, text); ) {
		  vector<int> positions = matchBoyerMoore(text.c_str(), pattern);

			for (int p : positions) {
				printf("%s:%lld:%d: %s\n", text_files[i], line, p, pattern);
			}

			line = line + 1;
		}
	}
}

void help() {
	printf("Uso: pmt [options] [pattern] textfile [textfile...]\n");
	printf("-h, --help\t\tGuia de ajuda\n");
	printf("-e, --edit d\t\tFaz uma busca aproximada em uma distância máxima 'd'\n");
	printf("-p, --pattern file\tUtiliza todos os padrões em 'file' como entrada\n");

	exit(0);
}

int main(int argc, char **argv) {
	bool multi_pattern = false;
	bool approximate_matching = false;

	int edit_distance = 0;
	string patterns_file;
	vector<const char*> patterns;
	vector<const char*> text_files;

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

			edit_distance = optarg == NULL ? 0 : atoi(optarg);
			options += 2;

			break;
		case 'p':
			multi_pattern = true;

			patterns_file = optarg == NULL ? "" : optarg;
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

		patterns.push_back(argv[options]);
	} else {
		ifstream patterns_stream(patterns_file);

		if (!patterns_stream.good()) {
			printf("Arquivo inválido: '%s'.\n", patterns_file.c_str());
		}

		string pattern;

		while (patterns_stream >> pattern) {
			patterns.push_back(pattern.c_str());
		}
	}

	if (argc <= options + 1) {
		printf("Please, add one or more files in order to search.\n");
		help();
	}

	for (int i = options + 1; i < argc; i++) {
		vector<char*> files_name = getFilesName(argv[i]);

		for (unsigned int i = 0; i < files_name.size(); i++) {
			text_files.push_back(files_name.at(i));
		}
	}

	if (!approximate_matching) {
		if (patterns.size() == 1) {
			match_boyer_moore(text_files, patterns[0]);
		}
	}

	return 0;
}
