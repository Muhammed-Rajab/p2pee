#include "node.hpp"

#include "protocol.hpp"

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
    } else {
      handle_unknown(type, session);
    }

  } catch (const std::exception &e) {
    std::cout << "error parsing json" << std::endl;

    // XXX: Only do this on development. you don't need end users to know about
    // internal stuff
    json response = {{"type", "ERROR"},
                     {"message", std::string("invalid json: ") + e.what()}};
    session->send(response.dump() + "\n");
  }
}

// PRIVATE METHODS

void Protocol::handle_ping(std::shared_ptr<Session> session) {
  json response = {{"type", "PONG"}};
  session->send(response.dump() + "\n");
}

void Protocol::handle_unknown(const std::string &type,
                              std::shared_ptr<Session> session) {
  json response = {{"type", "ERROR"},
                   {"message", "unknown message type: " + type}};

  session->send(response.dump() + "\n");
}

void Protocol::handle_missing_type(std::shared_ptr<Session> session) {
  json response = {{"type", "ERROR"}, {"message", "missing type"}};

  session->send(response.dump() + "\n");
}
