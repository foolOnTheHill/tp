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
#include "sellers.h"
#include "wu-manber.h"

using namespace std;

/* Handles wildcards on the file name. */
vector<string> getFilesName(char* f) {
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

void match_boyer_moore(vector<string> text_files, string pattern) {
	long long line = 1LL;

	for (unsigned int i = 0; i < text_files.size(); i++) {
		string text_file = text_files.at(i);

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Invalid file: '%s'.\n", text_files.at(i).c_str());
		}

		for(string text; getline(text_stream, text); ) {
			vector<int> positions = matchBoyerMoore(text, pattern);

			for (int p : positions) {
				printf("%s:%lld:%d: %s\n", text_files[i].c_str(), line, p, pattern.c_str());
			}

			line = line + 1;
		}
	}
}

void match_aho_corasick(vector<string> text_files, vector<string> patterns) {
	long long line = 1LL;

	// AFD
	long long* out;
	int* f;
	int** g;

	int total_patterns_count = 0;
	unsigned int patterns_count = patterns.size();

	// printf("Padroes: %d\n", patterns_count);

	for (int i = 0; i < patterns_count; i++) {
		// printf("-- %s\n", patterns.at(i).c_str());
		total_patterns_count += patterns[i].length();
	}

	prepareAhoCorasick(patterns, f, g, out, total_patterns_count); // Creates the AFD only once

	for (unsigned int i = 0; i < text_files.size(); i++) {
		string text_file = text_files.at(i);

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Invalid file: '%s'.\n", text_files[i].c_str());
		}

		for (string text; getline(text_stream, text); ) {
			vector<int>* positions = matchAhoCorasick(text, patterns, f, g, out);

			for (unsigned int j = 0; j < patterns_count; j++) {
				for (unsigned int k = 0; k < positions[j].size(); k++) {
					printf("%s:%lld:%d: %s\n", text_file.c_str(), line, positions[j].at(k), patterns.at(j).c_str());
				}
			}

			line = line + 1;
		}
	}
}

void match_approximate(vector<string> text_files, vector<string> patterns, int e) {
	long long line = 1LL;

	for (unsigned int i = 0; i < text_files.size(); i++) {
		string text_file = text_files.at(i);

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Invalid file: '%s'.\n", text_files.at(i).c_str());
		}

		for(string text; getline(text_stream, text); ) {

			for (string &pat : patterns){
				vector<int> positions;

				if( (e < 10 && pat.length() < 63 ) || (e < 2 && pat.length() > 100) ) {
					positions = wu_manber(text, pat, e);
				} else {
					positions = sellers(text, pat, e);
				}

				for (int p : positions) {
					printf("%s:%lld:%d: %s\n", text_files[i].c_str(), line, p, pat.c_str());
				}
			}

			line = line + 1;
		}
	}
}

void help() {
	printf("Usage: pmt [options] [pattern] textfile [textfile...]\n");
	printf("-h, --help\t\tShows this\n");
	printf("-e, --edit d\t\tMakes an approximate matching using 'd' as maximum distance\n");
	printf("-p, --pattern file\tMakes the matching using each line in 'file' as a pattern\n");

	exit(0);
}

int main(int argc, char **argv) {
	bool multi_pattern = false;
	bool approximate_matching = false;

	int edit_distance = 0;
	string patterns_file;
	vector<string> patterns;
	vector<string> text_files;

	static struct option long_options[] = {
			{ "help", no_argument, 0, 'h' },
			{ "edit", required_argument, 0, 'e' },
			{ "pattern", required_argument, 0, 'p' },
			{ 0, 0, 0, 0 }
	};

	int option;
	int option_index = 0;
	int options = 0;

	while ((option = getopt_long(argc, argv, "he:p:", long_options, &option_index)) != -1) {
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
			printf("Invalid file: '%s'.\n", patterns_file.c_str());
		}

		for(string text; getline(patterns_stream, text); ) {
			patterns.push_back(text);
		}

	}

	if (argc <= options + 1) {
		printf("Please, add one or more files in order to search.\n");
		help();
	}

	for (int i = options + 1; i < argc; i++) {
		vector<string> files_name = getFilesName(argv[i]);

		for (unsigned int i = 0; i < files_name.size(); i++) {
			text_files.push_back(files_name.at(i));
		}
	}

	if (!approximate_matching) {
		if (patterns.size() == 1) {
			match_boyer_moore(text_files, patterns.at(0));
		} else {
			match_aho_corasick(text_files, patterns);
		}
	} else {
		match_approximate(text_files, patterns, edit_distance);
	}

	return 0;
}
