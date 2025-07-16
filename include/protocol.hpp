#ifndef PROTOCOL_4_P2PEE_HPP

#include <string>

#include "node.hpp"
#include "server.hpp"

#include <nlohmann/json.hpp>

class ServerProtocol {

private:
  Node &node;

  void handle_ping(std::shared_ptr<Session> session);
  void handle_hello(const json &data, std::shared_ptr<Session> session);
  void handle_peers(const json &data, std::shared_ptr<Session> session);

  void handle_unknown(const std::string &type,
                      std::shared_ptr<Session> session);
  void handle_missing_type(std::shared_ptr<Session> session);

public:
  ServerProtocol(Node &node_);
  void handle(const std::string &message, std::shared_ptr<Session> session);
};

#endif // !PROTOCOL_4_P2PEE_HPP
