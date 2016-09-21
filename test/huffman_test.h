#ifndef HUFFMAN_TEST_H
#define HUFFMAN_TEST_H
#include "autotest.h"
#include <string>

using namespace std;

const size_t SZ = 1 << 20;

class HuffmanTest : public Tester {
  private:
  size_t sz;
  static void save_test(string&);
  static bool is_equal(ifstream&, ifstream&);

  public:
  HuffmanTest();
  ~HuffmanTest();
  static bool test1();
  static bool test2();
  static bool test3();
  static bool validate(string&);
};
#endif
