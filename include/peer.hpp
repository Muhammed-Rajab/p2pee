#ifndef PEER_4_P2PEE_HPP
#define PEER_4_P2PEE_HPP

#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>

struct Peer {
  std::string id;
  std::string address;
  uint16_t port;
  std::string public_key;

  Peer() = default;

  Peer(const std::string &id_, const std::string &address_, uint16_t port_,
       const std::string &public_key_)
      : id(id_), address(address_), port(port_), public_key(public_key_) {}

  std::string to_string() const {
    std::ostringstream oss;
    oss << "Peer{id=" << id << ", address=" << address << ", port=" << port
        << ", public_key=" << public_key << "}";
    return oss.str();
  }

  bool operator==(const Peer &other) const { return this->id == other.id; }

  // TODO: add a method to verify the ID by hashing the public_key
};

void to_json(nlohmann::json &j, const Peer &peer) {
  j = nlohmann::json{
      {"id", peer.id},
      {"address", peer.address},
      {"port", peer.port},
      {"public_key", peer.public_key},
  };
}

#endif // !PEER_4_P2PEE_HPP
