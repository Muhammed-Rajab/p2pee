#ifndef NODE_4_P2PEE_HPP
#define NODE_4_P2PEE_HPP

#include <boost/asio/io_context.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <thread>
#include <unordered_map>

#include "keypair.hpp"
#include "peer.hpp"
#include "server.hpp"

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

  void handle_message(const std::string &msg,
                      std::shared_ptr<Session> session) {

    std::istringstream iss(msg);
    std::string command;
    iss >> command;

    if (command == "PING") {
      session->send("PONG\n");
    } else if (command == "IDENTIFY") {
      session->send("HELLO id=" + id + " address=" + address +
                    " port=" + std::to_string(port) + "\n");
    } else if (command == "PEERS") {
      session->send("peer list will be sent soon\n");
    } else {
      session->send("ERROR unknown_command\n");
    }
  }

public:
  Node(const std::string &private_key_path, const std::string &public_key_path,
       short port_)
      : keypair(private_key_path, public_key_path), port(port_),
        server(
            io_context, port,
            [this](const std::string &msg, std::shared_ptr<Session> session) {
              this->handle_message(msg, session);
            }) {

    // verify
    if (!keypair.verify_pair()) {
      throw std::runtime_error("keypair verification failed. provide valid "
                               "public/private keypairs.");
    }

    // generate id
    id = keypair.generate_id();

    // XXX: placeholdering address and port for testing purposes
    address = "127.0.0.1";

    // create empty known peers
    known_peers = {};

    // start server in bg
    server_thread = std::thread([this]() {
      std::cout << "[Node] running at " << address << ":" << port << "\n";
      io_context.run();
    });
  }

  ~Node() {
    io_context.stop();
    if (server_thread.joinable())
      server_thread.join();
  }

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
  void add_peer(const Peer &peer) {
    if (peer.id == id)
      return;
    known_peers[peer.id] = peer;
  }
  void remove_peer(const std::string &peer_id) { known_peers.erase(peer_id); }

  // to string
  std::string to_string() const {
    std::ostringstream oss;
    oss << "Node{id=" << id << ", address=" << address << ", port=" << port
        << ", known_peers=" << known_peers.size() << "}\n";
    for (const auto &entry : known_peers) {
      oss << "  - " << entry.second.to_string() << "\n";
    }
    return oss.str();
  }
};

#endif // !NODE_4_P2PEE_HPP
