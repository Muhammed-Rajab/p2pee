#ifndef NODE_4_P2PEE_HPP
#define NODE_4_P2PEE_HPP

#include <iostream>
#include <stdexcept>
#include <string>

#include "keypair.hpp"

class Node {
private:
  std::string id;
  KeyPair keypair;

public:
  Node(const std::string &private_key_path, const std::string &public_key_path)
      : keypair(private_key_path, public_key_path) {

    // verify
    if (!keypair.verify_pair()) {
      throw std::runtime_error("keypair verification failed. provide valid "
                               "public/private keypairs.");
    }

    // generate id
    id = keypair.generate_id();
  }

  ~Node() {}

  std::string get_id() const { return id; }
};

#endif // !NODE_4_P2PEE_HPP
