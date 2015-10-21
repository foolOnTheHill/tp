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

void print_matching(const string text_file, long long line, int position, const string pattern) {
	printf("%s:%lld:%d: %s\n", text_file.c_str(), line, position, pattern.c_str());
}

void print_count(const string text_file, long long count, const string pattern) {
    printf("%s:%lld: %s\n", text_file.c_str(), count, pattern.c_str());
}

void match_boyer_moore(vector<string> text_files, const string pattern, bool only_counting) {
	int m = pattern.length();

	int* occ = new int[ALPHABET_LENGTH]; // Occurrence function for pattern
	int* f = new int[m+1]; // Starting position of the widest border of the suffix of the pattern beginning at position i
	int* s = new int[m+1]; // The corresponding shift distance is saved in an array s

	prepareBoyerMoore(pattern, occ, f, s);

	for (int i = 0, n = text_files.size(); i < n; i++) {
		long long line = 1LL;
		long long count = 0LL;

		string text_file = text_files[i];

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Invalid file: '%s'.\n", text_file.c_str());
		}

		for(string text; getline(text_stream, text); ) {
			vector<int> positions = matchBoyerMoore(text, pattern, occ, f, s);

			if (!only_counting) {
				for (int j = 0, o = positions.size(); j < o; j++) {
					print_matching(text_file, line, positions[j], pattern);
				}

				line = line + 1;
			} else {
				count = count + positions.size();
			}
		}

		if (only_counting) {
			print_count(text_file, count, pattern);
		}
	}
}

void match_aho_corasick(vector<string> text_files, vector<string> patterns, bool only_counting) {
	// AFD
	long long* out;
	int* f;
	int** g;

	int total_patterns_count = 0;
	int pattern_count = patterns.size();

	for (int i = 0; i < pattern_count; i++) {
		total_patterns_count += patterns[i].length();
	}

	prepareAhoCorasick(patterns, f, g, out, total_patterns_count); // Creates the AFD only once

	for (int i = 0, n = text_files.size(); i < n; i++) {
		long long line = 1LL;
		long long count[pattern_count];

		fill(count, count + pattern_count, 0LL);

		string text_file = text_files[i];

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Invalid file: '%s'.\n", text_file.c_str());
		}

		for (string text; getline(text_stream, text); ) {
			vector<int>* positions = matchAhoCorasick(text, patterns, f, g, out);

			if (!only_counting) {
				for (int j = 0; j < pattern_count; j++) {
					for (int k = 0, o = positions[j].size(); k < o; k++) {
						print_matching(text_file, line, positions[j][k], patterns[j]);
					}
				}

				line = line + 1;
			} else {
				for (int j = 0; j < pattern_count; j++) {
					count[j] = count[j] + positions[j].size();
				}
			}
		}

		if (only_counting) {
			for (int j = 0; j < pattern_count; j++) {
				print_count(text_file, count[j], patterns[j]);
			}
		}
	}
}

void match_approximate(vector<string> text_files, vector<string> patterns, int e, bool only_counting) {
    for (int i = 0, n = text_files.size(); i < n; i++) {
		int pattern_count = patterns.size();

		long long line = 1LL;
		long long count[pattern_count];

		fill(count, count + pattern_count, 0LL);

		string text_file = text_files[i];

		ifstream text_stream(text_file);

		if (!text_stream.good()) {
			printf("Invalid file: '%s'.\n", text_file.c_str());
		}

		for(string text; getline(text_stream, text); ) {
			for (int j = 0; j < pattern_count; j++){
				vector<int> positions;

				string pat = patterns[j];

				if((e < 10 && pat.length() < 63) || (e < 2 && pat.length() > 100)) {
					positions = wu_manber(text, pat, e);
				} else {
					positions = sellers(text, pat, e);
				}

				if (!only_counting) {
					for (int k = 0, o = positions.size(); k < o; k++) {
						print_matching(text_file, line, positions[k], pat);
					}
				} else {
					count[j] = count[j] + positions.size();
				}
			}

			if (!only_counting) {
				line = line + 1;
			}
		}

		if (only_counting) {
			for (int j = 0; j < pattern_count; j++) {
				print_count(text_file, count[j], patterns[j]);
			}
		}
	}
}

void show_help() {
	printf("Usage: pmt [options] [pattern] textfile [textfile...]\n");
	printf("-h, --help\t\tShows this\n");
	printf("-e, --edit d\t\tMakes an approximate matching using 'd' as maximum distance\n");
	printf("-p, --pattern file\tMakes the matching using each line in 'file' as a pattern\n");
	printf("-c, \t\t\tOnly prints how many times the patterns match on each line in 'file'\n");
	exit(0);
}

void show_usage() {
	printf("Usage: pmt [options] [pattern] textfile [textfile...]\n");
	exit(0);
}

int main(int argc, char** argv) {
	bool multi_pattern = false;
	bool approximate_matching = false;
	bool only_counting = false;

	int edit_distance = 0;
	string patterns_file;
	vector<string> patterns;
	vector<string> text_files;

	static struct option long_options[] = {
		{ "help", no_argument, 0, 'h' },
		{ "edit", required_argument, 0, 'e' },
		{ "pattern", required_argument, 0, 'p' },
		{ "count", no_argument, 0, 'c'},
		{ 0, 0, 0, 0 }
	};

	int option;
	int option_index = 0;
	int arg_index = 1;

	while ((option = getopt_long(argc, argv, "he:p:c", long_options, &option_index)) != -1) {
		switch (option) {
		case 'h':
			show_help();

			break;
		case 'e':
			approximate_matching = true;

			if (optarg == NULL) {
				show_usage();
			}

			edit_distance = atoi(optarg);
			arg_index = arg_index + 2;

			break;
		case 'p':
			multi_pattern = true;

			if (optarg == NULL) {
				show_usage();
			}

			patterns_file = optarg;
			arg_index = arg_index + 2;

			break;
		case 'c':
			only_counting = true;

			arg_index += 1;

			break;
		default:
			show_usage();

			break;
		}
	}

	if (arg_index >= argc) {
		show_usage();
	}

	if (!multi_pattern) {
		patterns.push_back(argv[arg_index]);

		arg_index = arg_index + 1;
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

	if (!approximate_matching) {
		if (patterns.size() == 1) {
			match_boyer_moore(text_files, patterns[0], only_counting);
		} else {
			match_aho_corasick(text_files, patterns, only_counting);
		}
	} else {
		match_approximate(text_files, patterns, edit_distance, only_counting);
	}

	return 0;
}
