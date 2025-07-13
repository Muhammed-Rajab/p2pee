#ifndef NODE_4_P2PEE_HPP
#define NODE_4_P2PEE_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "keypair.hpp"
#include "peer.hpp"

class Node {
private:
  std::string id;
  KeyPair keypair;
  std::unordered_map<std::string, Peer> known_peers;

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

    // create empty known peers
    known_peers = {};
  }

  ~Node() {}

  std::string get_id() const { return id; }

  const KeyPair &get_keypair() const { return keypair; }

  // peer management
  void add_peer(const Peer &peer) { known_peers[peer.id] = peer; }

  void remove_peer(const std::string &peer_id) { known_peers.erase(peer_id); }

  const std::unordered_map<std::string, Peer> &get_peers() const {
    return known_peers;
  }
};

#endif // !NODE_4_P2PEE_HPP
