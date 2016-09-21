#include "huffman_test.h"
#include <iostream>

int main()
{
  HuffmanTest* test = new HuffmanTest();
  test->show_stats();
  delete test;
}
