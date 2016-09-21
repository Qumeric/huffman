#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "bitchar.cpp"
#include "node.cpp"
#include "packer.cpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Huffman {
  private:
  vector<char> data;
  vector<string> codes;
  vector<size_t> freq;
  Packer* packer;
  Node* tree;

  ifstream& in;
  ofstream& out;

  void build_freq_by_data();
  void build_tree();
  void get_codes(Node*, string s = "");
  void readEncoded(Node*, string&, size_t&);
  void delete_tree(Node*);

  public:
  Huffman(Packer*, ifstream&, ofstream&);
  ~Huffman();
  void show_codes();
  void decode();
  void encode();
};
#endif
