#ifndef NODE_4_P2PEE_HPP
#define NODE_4_P2PEE_HPP

#include <boost/asio/io_context.hpp>
#include <memory>
#include <string>
#include <sys/types.h>
#include <thread>
#include <unordered_map>

#include "keypair.hpp"
#include "peer.hpp"
#include "server.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Node {
private:
  std::string id;
  unsigned short port;
  std::string address;

  KeyPair keypair;
  std::unordered_map<std::string, Peer> known_peers;

  // server related
  boost::asio::io_context io_context;
  Server server;
  std::thread server_thread;

  // protocol and message handling related
  void handle_message(const std::string &msg, std::shared_ptr<Session> session);

public:
  Node(const std::string &private_key_path, const std::string &public_key_path,
       short port_);
  ~Node();

  // getters
  std::string get_id() const;
  uint16_t get_port() const;
  std::string get_address() const;
  const KeyPair &get_keypair() const;
  const std::unordered_map<std::string, Peer> &get_known_peers() const;

  // conversion
  Peer to_peer() const;

  // peer management
  void add_peer(const Peer &peer);
  void remove_peer(const std::string &peer_id);

  // to string
  std::string to_string() const;
};

#endif // !NODE_4_P2PEE_HPP
