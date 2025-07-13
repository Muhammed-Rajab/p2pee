#ifndef NODE_4_P2PEE_HPP
#define NODE_4_P2PEE_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unordered_map>

#include "keypair.hpp"
#include "peer.hpp"

class Node {
private:
  std::string id;
  uint16_t port;
  std::string address;

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

    // XXX: placeholdering address and port for testing purposes
    address = "127.0.0.1";
    port = 6969;

    // create empty known peers
    known_peers = {};
  }

  ~Node() {}

  // getters
  std::string get_id() const { return id; }
  uint16_t get_port() const { return port; }
  std::string get_address() const { return address; }
  const KeyPair &get_keypair() const { return keypair; }
  const std::unordered_map<std::string, Peer> &get_known_peers() const {
    return known_peers;
  }

  // conversion
  Peer as_peer() const { return Peer{id, address, port}; }

  // peer management
  void add_peer(const Peer &peer) { known_peers[peer.id] = peer; }
  void remove_peer(const std::string &peer_id) { known_peers.erase(peer_id); }

  // to string
  std::string to_string() const {
    std::ostringstream oss;
    oss << "Node{id=" << id << ", known_peers=" << known_peers.size() << "}\n";
    for (const auto &entry : known_peers) {
      oss << "  " << entry.second.to_string();
    }
    return oss.str();
  }
};

#endif // !NODE_4_P2PEE_HPP
