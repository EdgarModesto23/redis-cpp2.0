#include "server.hpp"
#include "bulkstring.hpp"
#include "command.hpp"
#include "router.hpp"
#include "spdlog/spdlog.h"
#include <arpa/inet.h>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/placeholders.hpp>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using asio::ip::tcp;

int get_num_commands(const char *req, size_t length) {
  if (length < 4 || req[0] != '*') {
    throw std::runtime_error("Invalid RESP request: missing '*'");
  }

  const char *p = req + 1; // start after '*'
  const char *end = req + length;

  // Parse digits until CR
  int num = 0;
  bool hasDigits = false;

  while (p < end && *p != '\r') {
    if (!std::isdigit(*p)) {
      throw std::runtime_error("Invalid RESP array length: non-digit");
    }
    hasDigits = true;
    num = num * 10 + (*p - '0');
    p++;
  }

  if (!hasDigits)
    throw std::runtime_error("Missing array length");

  // after digits, must be "\r\n"
  if (p + 1 >= end || p[0] != '\r' || p[1] != '\n') {
    throw std::runtime_error("Invalid RESP array header CRLF");
  }

  return num;
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
  typedef std::shared_ptr<tcp_connection> pointer;

  static pointer create(asio::io_context &io_context, Server &server) {
    return pointer(new tcp_connection(io_context, server));
  }

  tcp::socket &socket() { return socket_; }

  void start() { do_read(); }

private:
  tcp_connection(asio::io_context &io_context, Server &srv)
      : socket_(io_context), server_(srv) {}

  std::vector<std::unique_ptr<ICommand>>
  parse_request(const char *req, size_t num_commands, size_t length) {
    std::vector<std::unique_ptr<ICommand>> result{};
    const char *cursor = req;
    size_t remaining = length;

    for (size_t i = 0; i < num_commands; i++) {
      size_t consumed = 0;

      BulkString cmd = BulkString::decode(cursor, remaining, consumed);

      auto command = router::get_command(cmd, server_, message_);

      result.push_back(std::move(command));

      cursor += consumed;
      remaining -= consumed;
    }

    return result;
  }

  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        asio::buffer(data_),
        [this, self](std::error_code ec, std::size_t length) {
          if (!ec) {
            handle_request(std::string(data_.data(), length), this->server_);
            do_write();
          }
        });
  }

  void handle_request(const std::string &req, Server &server) {
    int n_commands = get_num_commands(req.c_str(), req.length());

    const char *cursor = req.c_str() + 1;

    while (*cursor != '\r')
      cursor++;
    cursor += 2;

    size_t remaining = req.length() - (cursor - req.c_str());

    auto commands = parse_request(cursor, n_commands, remaining);

    for (auto &cmd : commands) {
      cmd->serveRequest();
    }
  }

  void do_write() {
    auto self(shared_from_this());
    spdlog::info(message_);
    asio::async_write(socket_, asio::buffer(message_),
                      [this, self](std::error_code ec, std::size_t) {
                        if (!ec) {
                          do_read();
                        }
                      });
  }

  tcp::socket socket_;
  std::array<char, 1024> data_;
  std::string message_;
  Server &server_;
};

void handle_accept(Server &srv, tcp_connection::pointer con,
                   const std::error_code &err) {
  if (!err) {
    spdlog::info("Client connected");

    con->start();

  } else {
    spdlog::error("Accept failed");
  }

  srv.accept();
}

void Server::accept() {

  tcp_connection::pointer new_conn = tcp_connection::create(ctx_, *this);
  acceptor_.async_accept(new_conn->socket(),
                         [this, new_conn](const std::error_code &err) {
                           handle_accept(*this, new_conn, err);
                         });
}

int Server::init_server() {

  spdlog::info("Listening on port 6379...");
  spdlog::debug("Waiting for a client to connect...");
  accept();

  ctx_.run();

  return 0;
}
