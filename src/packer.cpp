#ifndef PACKER_CPP
#define PACKER_CPP
#include "node.cpp"
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define BYTESIZE(x) (x + 7) / 8

const size_t BLOCK_SZ = 8;
const unsigned char CODE_LOGSZ = 5;
const size_t MAX_ALPH_SZ = 1 << BLOCK_SZ;

class Packer {
  public:
  virtual ~Packer(){};
  virtual string pack(vector<string>&, vector<unsigned char>&) = 0;
  virtual Node* unpack(string&, size_t&, size_t) = 0;
};

class SimplePacker : public Packer {

  public:
  string pack(vector<string>& codes, vector<unsigned char>& data)
  {
    string align, header, msg;
    unsigned alph_size = 0;
    for (size_t i = 0; i < MAX_ALPH_SZ; i++) {
      if (codes[i] != "")
        alph_size++;
    }
    header += bitset<9>(alph_size).to_string(); // One bit isn't worth tricks
    for (size_t i = 0; i < MAX_ALPH_SZ; i++) {
      if (codes[i] != "") {
        header += bitset<BLOCK_SZ>(i).to_string();
        header += bitset<CODE_LOGSZ>(codes[i].size()).to_string();
        header += codes[i];
      }
    }

    for (unsigned char c : data)
      msg += codes[c];

    size_t align_len = 8 - (header.size() + msg.size()) % 8;
    for (size_t i = 1; i < align_len; i++) {
      align += '0';
    }
    align += '1';

    cout << data.size() << endl;
    cout << BYTESIZE(align.size() + header.size() + msg.size()) << endl;
    cout << BYTESIZE(align_len + header.size()) << endl;

    return align + header + msg;
  }

  Node* unpack(string& data, size_t& pos, size_t file_size)
  {
    Node* trie = new Node;
    while (data[pos++] == '0')
      ;
    unsigned alph_size = stoi(data.substr(pos, 9), nullptr, 2);
    pos += 9;
    while (alph_size--) {
      unsigned char c = stoi(data.substr(pos, BLOCK_SZ), nullptr, 2);
      pos += BLOCK_SZ;
      unsigned char sz = stoi(data.substr(pos, CODE_LOGSZ), nullptr, 2);
      pos += CODE_LOGSZ;
      string code = data.substr(pos, sz);
      pos += sz;
      trie->add(code, c);
    }
    return trie;
  }
};
#endif
