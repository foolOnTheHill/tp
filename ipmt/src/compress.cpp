#include "compress.h"

void handle_compression(string &textfile, string &comp_file, string compressAlg) {
  if (compressAlg.compare("lzw") == 0) {
    compress_lzw(textfile, comp_file);
  } else if (compressAlg.compare("lz77") == 0) {
    compress_lz77(textfile, comp_file);
  } else {
    printf("Invalid compression algorithm '%s'.\n", compressAlg.c_str());
    exit(1);
  }
}

void handle_decompression(string &comp_file, string &decode) {
  ifstream ifs(comp_file.c_str());

  if (!ifs.good()) {
    printf("Invalid pattern file '%s'!\n", comp_file.c_str());
    exit(1);
  }

  char c_aux;
  string compressAlg = "";

  ifs >> c_aux;
  compressAlg += c_aux;
  ifs >> c_aux;
  compressAlg += c_aux;
  ifs >> c_aux;
  compressAlg += c_aux;

  if (compressAlg.compare("LZW") == 0) {
    decompress_lzw(comp_file, decode);
  } else if (compressAlg.compare("LZ7") == 0) {
    decompress_lz77(comp_file, decode);
  } else {
    printf("Invalid compression algorithm '%s'.\n", compressAlg.c_str());
    exit(1);
  }
}
