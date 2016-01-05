#ifndef _IPMT_COMPRESS
#define _IPMT_COMPRESS

#include "lzw.h"
#include "lz77.h"

using namespace std;

void handle_compression(string &textfile, string &comp_file, string compressAlg);
void handle_decompression(string &comp_file, string &decode);

#endif
