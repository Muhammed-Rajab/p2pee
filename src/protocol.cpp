#include "nlohmann/json_fwd.hpp"
#include "node.hpp"
#include <exception>
#include <string>

#include "protocol.hpp"

using json = nlohmann::json;

// PUBLIC METHODS
Protocol::Protocol(Node &node_) : node(node_) {}

void Protocol::handle(const std::string &message,
                      std::shared_ptr<Session> session) {

  /*
   * XXX:
   * RULES
   * 1. every interaction happens in JSON. from PING to peer discovery
   *
   * TODO:
   * DESIGN
   * structure: {"type": string, "data": any}
   * Types:
   *  - PING
   *    * Returns {"type": "PONG"}
   * */

  try {
    auto j = json::parse(message);

    if (!j.contains("type")) {
      handle_missing_type(session);
      return;
    }

    std::string type = j["type"];
    const auto &data = j.value("data", json::object());

    if (type == "PING") {
      handle_ping(session);
    } else if (type == "HELLO") {
      handle_hello(data, session);
    } else if (type == "PEERS") {
      handle_peers(data, session);
    } else {
      handle_unknown(type, session);
    }
  } catch (const std::exception &e) {
    std::cout << "error parsing json" << std::endl;

    // XXX: Only do this on development. you don't need end users to know
    // about internal stuff
    json response = {{"type", "ERROR"},
                     {"message", std::string("invalid json: ") + e.what()}};
    session->send(response.dump() + "\n");
  }
}

// PRIVATE METHODS

// PING
void Protocol::handle_ping(std::shared_ptr<Session> session) {
  json response = {{"type", "PONG"}};
  session->send(response.dump() + "\n");
}

// HELLO
void Protocol::handle_hello(const json &data,
                            std::shared_ptr<Session> session) {
  session->send("heres hi!\n");
  // receives id, addr, port and public key of peer
  // sent WELCOME back to client with id

  // get all data
  try {
    if (!data.contains("id") || !data.contains("address") ||
        !data.contains("port") || !data.contains("public_key")) {
      handle_unknown("INVALID_HELLO_FIELDS", session);
      return;
    }

    // extract fields
    std::string peer_id = data["id"];
    std::string address = data["address"];
    unsigned short port = data["port"];
    std::string public_key = data["public_key"];

    std::cout << "[Protocol] Received HELLO from " << peer_id << " at "
              << address << ":" << port << std::endl;

    Peer peer(peer_id, address, port, public_key);

    // TODO: verify id with public key

    // add to peers
    node.add_peer(peer);

    // respond with WELCOME and penelope as Peer
    json node_json = node.to_peer();
    json response = {{"type", "WELCOME"}, {"data", node_json}};

    session->send(response.dump() + "\n");

  } catch (std::exception &e) {
    std::cerr << "Error handling HELLO: " << e.what() << std::endl;
    json error = {{"type", "ERROR"},
                  {"message", "exception: " + std::string(e.what())}};
    session->send(error.dump() + "\n");
  }
}

// PEERS
void Protocol::handle_peers(const json &data,
                            std::shared_ptr<Session> session) {
  session->send("heres peers!\n");
  // receives request for peers
  // sent PEERS back to client list of peer details
}

// UNKNOWN
void Protocol::handle_unknown(const std::string &type,
                              std::shared_ptr<Session> session) {
  json response = {{"type", "ERROR"},
                   {"message", "unknown message type: " + type}};

  session->send(response.dump() + "\n");
}

// MISSING TYPE
void Protocol::handle_missing_type(std::shared_ptr<Session> session) {
  json response = {{"type", "ERROR"}, {"message", "missing type"}};

  session->send(response.dump() + "\n");
}
