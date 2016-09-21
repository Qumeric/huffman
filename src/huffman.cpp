#include "huffman.h"
#include <fstream>
#include <new>
#include <queue>

namespace DEBUG {
void show_tree(Node* tree, size_t spacing = 0)
{
  if (!tree)
    return;
  for (size_t i = 0; i < spacing; i++)
    cerr << ' ';
  if (tree->is_end)
    cerr << tree->ending << endl;
  show_tree(tree->l, spacing + 1);
  show_tree(tree->r, spacing + 1);
}

void dfs(Node* tree, int& MAX_DEPTH, int depth = 0)
{
  MAX_DEPTH = max(MAX_DEPTH, depth);
  if (!tree)
    return;
  dfs(tree->l, MAX_DEPTH, depth + 1);
  dfs(tree->r, MAX_DEPTH, depth + 1);
}

void tree_depth(Node* tree)
{
  int MAX_DEPTH = 0;
  dfs(tree, MAX_DEPTH);
  cerr << "Tree depth is " << MAX_DEPTH << endl;
}
};

Huffman::Huffman(Packer* packer, ifstream& in, ofstream& out)
    : packer(packer)
    , in(in)
    , out(out)
{
  freq.resize(MAX_ALPH_SZ);
  codes.resize(MAX_ALPH_SZ);
}

void Huffman::encode()
{
  vector<unsigned char> buffer((
                                   istreambuf_iterator<char>(in)),
      (istreambuf_iterator<char>()));

  size_t len = 0;
  for (unsigned char i : buffer) {
    ++freq[i];
    ++len;
  }

  if (!len) {
    cerr << "Empty file!" << endl;
    throw;
  }

  build_tree();
  get_codes(tree);

  string encoded = packer->pack(codes, buffer);

  bitChar bchar;
  bchar.bits = encoded;
  bchar.insertBits(out);
}

void Huffman::decode()
{
  bitChar bchar;
  string buffer = bchar.readByBits(in);
  size_t pos = 0;
  in.seekg(0, ios_base::end);
  size_t file_size = in.tellg();
  if (file_size == 0) {
    cerr << "Empty file!" << endl;
    throw;
  }
  cout << file_size << endl;
  in.seekg(0, ios_base::beg);
  tree = packer->unpack(buffer, pos, file_size);
  size_t header_len = pos;
  size_t decoded_len = 0;
  while (pos < buffer.size()) {
    readEncoded(tree, buffer, pos);
    ++decoded_len;
  }
  cout << decoded_len << endl;
  cout << BYTESIZE(header_len) << endl;
}

void Huffman::readEncoded(Node* trie, string& code, size_t& pos)
{
  while (!trie->is_end) {
    if (code[pos] == '0') {
      trie = trie->l;
    } else {
      trie = trie->r;
    }
    ++pos;
  }
  out << trie->ending;
}

void Huffman::build_tree()
{
  priority_queue<Node*, vector<Node*>, Node_cmp> heap;
  for (size_t i = 0; i < MAX_ALPH_SZ; i++) {
    if (freq[i]) {
      try {
        heap.push(new Node(freq[i], i));
      } catch (bad_alloc e) {
        cerr << "Out of memory!" << endl;
        throw;
      }
    }
  }
  while (heap.size() > 1) {
    Node* cur1 = heap.top();
    heap.pop();
    Node* cur2 = heap.top();
    heap.pop();
    try {
      heap.push(new Node(cur1, cur2));
    } catch (bad_alloc e) {
      cerr << "Out of memory!" << endl;
      throw;
    }
  }
  tree = heap.top();
}

void Huffman::get_codes(Node* tree, string s)
{
  if (tree->is_end) {
    if (s == "")
      s = "0";
    codes[tree->ending] = s;
  } else {
    get_codes(tree->l, s + "0");
    get_codes(tree->r, s + "1");
  }
}

void Huffman::show_codes()
{
  for (size_t i = 0; i < MAX_ALPH_SZ; i++) {
    if (codes[i] != "")
      cout << char(i) << ": " << codes[i] << '\n';
  }
}

Huffman::~Huffman()
{
  delete_tree(tree);
  in.close();
  out.close();
}

void Huffman::delete_tree(Node* tree)
{
  if (!tree)
    return;
  delete_tree(tree->l);
  delete_tree(tree->r);
  delete tree;
}
