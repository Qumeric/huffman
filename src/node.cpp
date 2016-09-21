#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <new>
#include <string>

using namespace std;

struct Node {
  size_t w;
  bool is_end;
  unsigned char ending;
  Node *l, *r;
  Node(size_t w, unsigned char ending)
      : w(w)
      , ending(ending)
  {
    l = r = nullptr;
    is_end = true;
  }

  Node(Node* l, Node* r)
      : l(l)
      , r(r)
  {
    w = l->w + r->w;
    is_end = false;
  }

  Node()
  {
    l = r = nullptr;
    is_end = false;
  }

  void add(std::string code, char ch, size_t pos = 0)
  {
    if (pos == code.size()) {
      ending = ch;
      is_end = true;
      return;
    }
    try {
      if (code[pos] == '0') {
        if (!l)
          l = new Node();
        l->add(code, ch, pos + 1);
      } else {
        if (!r)
          r = new Node();
        r->add(code, ch, pos + 1);
      }
    } catch (bad_alloc e) {
      cerr << "Out of memory!" << endl;
      throw;
    }
  }
};

struct Node_cmp {
  bool operator()(const Node* l, const Node* r) const
  {
    return l->w > r->w;
  }
};
#endif
