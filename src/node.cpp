#include "node.hpp"

#include "protocol.hpp"

// PUBLIC
Node::Node(const std::string &private_key_path,
           const std::string &public_key_path, short port_)
    : keypair(private_key_path, public_key_path), port(port_),
      server(io_context, port,
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

Node::~Node() {
  io_context.stop();
  if (server_thread.joinable())
    server_thread.join();
}

// getters
std::string Node::get_id() const { return id; }
uint16_t Node::get_port() const { return port; }
std::string Node::get_address() const { return address; }
const KeyPair &Node::get_keypair() const { return keypair; }
const std::unordered_map<std::string, Peer> &Node::get_known_peers() const {
  return known_peers;
}

// conversion
Peer Node::to_peer() const {
  return Peer{id, address, port, keypair.get_public_as_string()};
}

// peer management
void Node::add_peer(const Peer &peer) {
  if (peer.id == id)
    return;
  known_peers[peer.id] = peer;
}
void Node::remove_peer(const std::string &peer_id) {
  known_peers.erase(peer_id);
}

// to string
std::string Node::to_string() const {
  std::ostringstream oss;
  oss << "Node{id=" << id << ", address=" << address << ", port=" << port
      << ", known_peers=" << known_peers.size() << "}\n";
  for (const auto &entry : known_peers) {
    oss << "  - " << entry.second.to_string() << "\n";
  }
  return oss.str();
}

// PRIVATE
void Node::handle_message(const std::string &msg,
                          std::shared_ptr<Session> session) {
  // use protocol here
  ServerProtocol ptcol(*this);
  ptcol.handle(msg, session);
}
