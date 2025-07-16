#ifndef CLIENT_4_P2PEE_HPP
#define CLIENT_4_P2PEE_HPP

#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/detail/error_code.hpp>
#include <exception>
#include <iostream>
#include <istream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

using tcp = boost::asio::ip::tcp;

class Client {
private:
  tcp::resolver resolver;
  tcp::socket socket;

public:
  Client(boost::asio::io_context &io_context_)
      : resolver(tcp::resolver(io_context_)), socket(io_context_) {}
  ~Client() {}

  void connect_to(std::string address, unsigned short port) {
    if (socket.is_open())
      socket.close();

    auto endpoints = resolver.resolve(address, std::to_string(port));
    boost::asio::connect(socket, endpoints);
    // XXX: remove later
    std::cout << "Connected to server\n";
  }

  std::string send(const std::string &message) {
    try {
      // send data
      boost::asio::write(socket, boost::asio::buffer(message));

      // read response
      boost::asio::streambuf response_buf;
      boost::asio::read_until(socket, response_buf, "\n");
      std::istream response_stream(&response_buf);
      std::string response;
      std::getline(response_stream, response);

      return response;
    } catch (std::exception &e) {
      std::cerr << "exception inside client: " << e.what() << "\n";
      return "";
    }
  }

  void close() {
    if (socket.is_open()) {
      boost::system::error_code ec;

      socket.shutdown(tcp::socket::shutdown_both, ec);
      if (ec) {
        std::cerr << "error shutting down socket: " << ec.message()
                  << std::endl;
      }

      ec.clear();
      socket.close(ec);
      if (ec) {
        std::cerr << "error closing socket: " << ec.message() << std::endl;
      }
    }
  }
};

#endif // !CLIENT_4_P2PEE_HPP
