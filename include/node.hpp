#ifndef NODE_4_P2PEE_HPP
#define NODE_4_P2PEE_HPP

#include <iostream>
#include <string>

class Node {
private:
  std::string id;
  std::string public_key;
  std::string private_key;

public:
  Node() {}

  ~Node() {}
};

#endif // !NODE_4_P2PEE_HPP
