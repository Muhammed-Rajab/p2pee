#ifndef SERVER_4_P2PEE_HPP
#define SERVER_4_P2PEE_HPP

#include <functional>
#include <iostream>

#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
private:
  tcp::socket socket_;
  boost::asio::streambuf buffer_;
  std::function<void(const std::string &, std::shared_ptr<Session>)> handler_;

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
            handler_(line, self); // calls Node's handler
          } else {
            std::cout << "read error: " << ec.message() << std::endl;
          }
        });
  }

public:
  explicit Session(
      tcp::socket socket,
      std::function<void(const std::string &, std::shared_ptr<Session>)>
          handler)
      : socket_(std::move(socket)), handler_(handler) {}

  void start() {
    std::cout << "new session started!" << std::endl;
    do_read();
  }

  void send(const std::string &msg) {
    auto self = shared_from_this();

    boost::asio::async_write(
        socket_, boost::asio::buffer(msg),
        [this, self](boost::system::error_code ec, std::size_t /*length*/
        ) {
          if (!ec) {
            do_read(); // keep reading next message
          }
        });
  }
};

class Server {
private:
  tcp::acceptor acceptor_;
  std::function<void(const std::string &, std::shared_ptr<Session>)> handler_;

  void start_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
          if (!ec) {
            std::make_shared<Session>(std::move(socket), handler_)->start();
          } else {
            std::cerr << "accept error: " << ec.message() << std::endl;
          }

          start_accept();
        });
  }

public:
  Server(boost::asio::io_context &io_context, short port,
         std::function<void(const std::string &, std::shared_ptr<Session>)>
             handler)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
        handler_(handler) {
    start_accept();
  }
};

#endif // !SERVER_4_P2PEE_HPP
