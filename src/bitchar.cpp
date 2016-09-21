#ifndef BITCHAR_H
#define BITCHAR_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
struct bitChar {
  string bits;

  string readByBits(ifstream& inf)
  {
    string s = "";
    inf.seekg(0, std::ios::end);
    size_t size = inf.tellg();
    std::string buffer(size, ' ');
    inf.seekg(0);
    inf.read(&buffer[0], size);
    for (size_t i = 0; i < buffer.length(); i++)
      s += getBits(buffer[i]);
    return s;
  }

  void insertBits(ofstream& outf)
  {
    if (bits.length() % 8 != 0) {
      cerr << "Try to write not properly defined byte" << endl;
      throw;
    }
    unsigned char buf = 0;
    for (size_t pos = 0; pos < bits.length(); pos++) {
      buf |= (bits[pos] == '1');
      if (pos % 8 == 7) {
        try {
          outf << buf;
        } catch (...) {
          cerr << "Could not write into a file" << endl;
          throw;
        }
        buf = 0;
      }
      buf <<= 1;
    }
  }

  string getBits(unsigned char X)
  {
    stringstream itoa;
    for (size_t s = 7; s > 0; s--)
      itoa << ((X >> s) & 1);

    itoa << (X & 1);
    return itoa.str();
  }
};
#endif
