#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/execution/start.hpp>
#include <boost/asio/impl/read_until.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <ios>
#include <iostream>
#include <istream>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
private:
  tcp::socket socket_;
  boost::asio::streambuf buffer_;

  void do_read() {
    auto self = shared_from_this();

    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [this, self](boost::system::error_code ec,
                     std::size_t bytes_transferred) {
          if (!ec) {
            std::istream is(&buffer_);
            std::string line;
            std::getline(is, line);

            std::cout << "received: " << line << std::endl;
            do_write("echo: " + line + "\n");
          } else {
            std::cout << "client disconnected or error occured!" << std::endl;
          }
        });
  }

  void do_write(const std::string &msg) {
    auto self = shared_from_this();

    boost::asio::async_write(
        socket_, boost::asio::buffer(msg),
        [this, self](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            do_read(); // keep reading next message
          }
        });
  }

public:
  explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}

  void start() {
    std::cout << "new session started!" << std::endl;
    do_read();
  }
};

class Server {
private:
  tcp::acceptor acceptor_;

  void start_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
          if (!ec) {
            std::make_shared<Session>(std::move(socket))->start();
          }

          start_accept();
        });
  }

public:
  Server(boost::asio::io_context &io_context, short port)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
  }
};

int main() {

  std::cout << "Hello world!" << std::endl;

  try {
    boost::asio::io_context io_context;
    Server server(io_context, 12346);
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "error: " << e.what() << "\n";
  }
  return EXIT_SUCCESS;
}
