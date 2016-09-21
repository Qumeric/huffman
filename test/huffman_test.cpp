#include "huffman_test.h"
#include "../src/huffman.h"
#include "autotest.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <random>
#include <string>

using namespace std;

// Dangerous! Better use hashing
const char* testName = "test.text";
const char* encName = "encoded.data";
const char* outName = "out.text";

HuffmanTest::HuffmanTest()
    : Tester()
{
  run_test(&test1);
  run_test(&test2);
  run_test(&test3);
}

HuffmanTest::~HuffmanTest()
{
  remove(testName);
  remove(encName);
  remove(outName);
}

void HuffmanTest::save_test(string& data)
{
  ofstream outstream(testName, ofstream::binary);
  outstream << data;
  outstream.close();
}

bool HuffmanTest::test1()
{
  string data;
  for (size_t i = 0; i < SZ; i++) {
    data += char('a' + rand() % 26);
  }
  save_test(data);
  return validate(data);
}

bool HuffmanTest::test2()
{
  string data;
  for (size_t i = 0; i < SZ; i++) {
    data += char(rand() % 256);
  }
  save_test(data);
  return validate(data);
}

bool HuffmanTest::test3()
{
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  uniform_int_distribution<unsigned> distribution(1, SZ);

  string data;
  for (size_t i = 0; i < SZ; i++) {
    data += char(log2(distribution(generator)) + 0.5);
  }
  save_test(data);
  return validate(data);
}

bool HuffmanTest::validate(string& data)
{
  Packer* packer = new SimplePacker;

  ifstream test_in(testName, ifstream::binary);
  ofstream enc_out(encName, ofstream::binary);
  Huffman encoder(packer, test_in, enc_out);
  encoder.encode();
  enc_out.close();

  ifstream enc_in(encName, ifstream::binary);
  ofstream dec_out(outName, ofstream::binary);
  Huffman decoder(packer, enc_in, dec_out);
  decoder.decode();
  enc_in.close();
  dec_out.close();

  ifstream result_in(outName, ifstream::binary);
  bool retval = is_equal(test_in, result_in);

  delete packer;
  test_in.close();
  result_in.close();

  return retval;
}

const size_t MAX_SIZE = 1 << 26;
bool HuffmanTest::is_equal(ifstream& a, ifstream& b)
{
  size_t pos_a = a.tellg();
  size_t pos_b = b.tellg();

  char* bufferA = new char[MAX_SIZE];
  char* bufferB = new char[MAX_SIZE];

  bool retval = !memcmp(bufferA, bufferB, MAX_SIZE);

  delete[] bufferA;
  delete[] bufferB;

  a.seekg(pos_a);
  b.seekg(pos_b);

  return retval;
}
