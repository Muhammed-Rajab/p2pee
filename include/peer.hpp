#ifndef PEER_4_P2PEE_HPP
#define PEER_4_P2PEE_HPP

#include <cstdint>
#include <sstream>
#include <string>

struct Peer {
  std::string id;
  std::string address;
  uint16_t port;

  Peer() = default;

  Peer(const std::string &id_, const std::string &address_, uint16_t port_)
      : id(id_), address(address_), port(port_) {}

  std::string string() const {
    std::ostringstream oss;
    oss << "Peer{id=" << id << ", address=" << address << ", port=" << port
        << "}";
    return oss.str();
  }

  bool operator==(const Peer &other) const { return this->id == other.id; }
};

#endif // !PEER_4_P2PEE_HPP
