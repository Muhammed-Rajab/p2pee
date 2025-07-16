#include "nlohmann/json_fwd.hpp"
#include <boost/asio/io_context.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>

#include "include/client.hpp"

using json = nlohmann::json;

bool test_PING(Client &c) {
  try {
    c.connect_to("127.0.0.1", 6666);
  } catch (std::exception &e) {
    std::cerr << "failed to connect to server: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  try {
    json msg = {{"type", "PING"}};

    std::string response = c.send(msg.dump() + "\n");

    std::cout << "from server: " << response << std::endl;

    c.close();
    // std::cin.get();

  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

bool test_HELLO(Client &c) {
  try {
    c.connect_to("127.0.0.1", 6666);
  } catch (std::exception &e) {
    std::cerr << "failed to connect to server: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  try {
    json msg = {{"type", "HELLO"},
                {"data",
                 {{"id", "larry's ID"},
                  {"address", "larry's address"},
                  {"port", 6969},
                  {"public_key", "larry's public key"}}}};

    std::string response = c.send(msg.dump() + "\n");

    std::cout << "from server: " << response << std::endl;

    c.close();
    // std::cin.get();

  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

bool test_PEERS(Client &c) {
  try {
    c.connect_to("127.0.0.1", 6666);
  } catch (std::exception &e) {
    std::cerr << "failed to connect to server: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  try {
    json msg = {{"type", "PEERS"}};

    std::string response = c.send(msg.dump() + "\n");

    std::cout << "from server: " << response << std::endl;

    c.close();
    // std::cin.get();

  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int main() {
  boost::asio::io_context io_context;

  Client c(io_context);

  std::cout << "testing PING" << std::endl;
  test_PING(c);

  std::cout << "testing HELLO" << std::endl;
  test_HELLO(c);

  std::cout << "testing PEERS" << std::endl;
  test_PEERS(c);

  return EXIT_SUCCESS;
}
