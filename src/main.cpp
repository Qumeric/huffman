#include "huffman.h"
#include "node.cpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <unistd.h>

using namespace std;

enum Mode { encode,
  decode };

void print_help()
{
  cout << "Usage: ./huffman -f input -o output [-c/-u]" << endl;
  cout << "-c create archive" << endl;
  cout << "-u read archive" << endl;
}

int main(int argc, char** argv)
{
  if (argc != 6) {
    print_help();
    return 0;
  }

  string input, output;
  Mode mode;
  int c;
  while (1) {
    static struct option long_options[] = {
      { "file", required_argument, 0, 'f' },
      { "output", required_argument, 0, 'o' },
      { 0, 0, 0, 0 } // What is it I don't even
    };
    int option_index = 0;
    c = getopt_long(argc, argv, "cuf:o:", long_options, &option_index);
    if (c == -1)
      break;
    switch (c) {
    case 'c':
      mode = encode;
      break;
    case 'u':
      mode = decode;
      break;
    case 'f':
      input = optarg;
      break;
    case 'o':
      output = optarg;
      break;
    default:
      print_help();
      return 0;
    }
  }

  ifstream instream;
  ofstream outstream;
  try {
    instream.open(input, ifstream::binary);
    outstream.open(output, ofstream::binary);
  } catch (const exception& e) {
    cerr << "I/O error" << endl;
    cerr << "Exception caught: " << e.what() << endl;
  }

  Packer* packer = new SimplePacker;

  Huffman huffman(packer, instream, outstream);

  if (mode == encode) {
    huffman.encode();
  } else if (mode == decode) {
    huffman.decode();
  }

  delete packer;

  return 0;
}
