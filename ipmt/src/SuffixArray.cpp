#include <bits/stdc++.h>

using namespace std;

class SuffixArray {
public:
  int arraySize, stringLen;
  int *RA, *SA, *tmpRA, *tmpSA, *count;
  char *str;

  void csort(int k) {
    memset(count, 0, arraySize * sizeof(int));

    int index;
    for(int i = 0; i < stringLen; i++) {
      index = 0;
      if (i + k < stringLen) {
        index = RA[i + k];
      }
      count[index]++;
    }

    for(int i = 1; i < max(stringLen, 300); i++) {
      count[i] += count[i - 1];
    }

    for (int i = max(stringLen, 300); i >= 1; i--) {
      count[i] = count[i-1];
    }

    count[0] = 0;

    for (int i = 0; i < stringLen; i++) {
      index = 0;
      if (SA[i] + k < stringLen) {
        index = RA[ SA[i] + k ];
      }
      tmpSA[ count[index]++ ] = SA[i];
    }

    for (int i = 0; i < stringLen; i++) {
      SA[i] = tmpSA[i];
    }
  }

  void prepareSA() {
    for (int i = 0; i < stringLen; i++) {
      RA[i] = (unsigned char) str[i];
      SA[i] = i;
    }

    int k = 1;
    while (k < stringLen) {
      csort(k);
      csort(0);

      int r = 0;
      tmpRA[SA[0]] = 0;
      for(int i = 1; i < stringLen; i++) {
        if ( RA[ SA[i] ] != RA[ SA[i-1] ] || RA[ SA[i] + k ] != RA[ SA[i-1] + k ] ) {
          r++;
        }
        tmpRA[ SA[i] ] = r;
      }

      for(int i = 0; i < stringLen; i++) {
        RA[i] = tmpRA[i];
      }

      if( RA[ SA[ stringLen-1 ] ] == stringLen-1) {
        break;
      }

      k <<= 1;
    }
  }

  void init(string &word) {
    stringLen = word.size();
    arraySize = (2 * stringLen) + 400;

    strncpy(str, word.c_str(), stringLen);

    RA = new int[arraySize];
    SA = new int[arraySize];
    tmpRA = new int[arraySize];
    tmpSA = new int[arraySize];
    count = new int[arraySize];
    str = new char[arraySize];

    memset(RA,0, arraySize * sizeof(int));
    memset(SA,0, arraySize * sizeof(int));
    memset(tmpRA,0, arraySize * sizeof(int));
    memset(tmpSA,0, arraySize * sizeof(int));
    memset(count,0, arraySize * sizeof(int));
    memset(str,0, arraySize);
  }

  void getRepr(string &buffer) {
    buffer = "";

    ostringstream os;
    os << stringLen << "\n";

    for (int i = 0; i < stringLen; i++) {
      os << SA[i] << "\n";
    }
    buffer = os.str();
  }

  void loadRepr(string &buffer) {
    stringstream ss(buffer);

    ss >> stringLen;
    if (stringLen < 0) {
      stringLen = -stringLen;
    }

    for (int i = 0; i < stringLen; i++) {
      ss >> SA[i];
    }
  }

  SuffixArray(string &word) {
    init(word);
    prepareSA();
  }

  SuffixArray(string &word, string &buffer) {
    init(word);
    loadRepr(buffer);
  }

  ~SuffixArray() {
    delete[] RA;
    delete[] SA;
    delete[] tmpRA;
    delete[] tmpSA;
    delete[] count;
    delete[] str;
  }

  vector<int> match(string &pattern) {
    vector<int> occ;

    int start, end, s, m, e;

    s = 0;
    e = stringLen-1;
    while (s < e) {
      m = (s + e) / 2;
      if (strncmp(str + SA[m], pattern.c_str(), pattern.size()) >= 0) {
        e = m;
      } else {
        s = m + 1;
      }
    }
    start = s;

    if (strncmp(str + SA[s], pattern.c_str(), pattern.size()) != 0) {
      return occ;
    }

    s = 0;
    e = stringLen-1;

    s = 0;
    e = stringLen-1;
    while (s < e) {
      m = (s + e) / 2;
      if (strncmp(str + SA[m], pattern.c_str(), pattern.size()) > 0) {
        e = m;
      } else {
        s = m + 1;
      }
    }

    if (strncmp(str + SA[s], pattern.c_str(), pattern.size()) > 0) {
      e--;
    }
    end = e;

    for(int i = start; i <= end; i++) {
      occ.push_back(SA[i]);
    }
    sort(occ.begin(), occ.end());

    return occ;
  }

};
