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

  std::unique_ptr<ICommand> parse_command(const char *req, size_t length) {
    const char *cursor = req;
    size_t remaining = length;

    if (remaining < 4 || cursor[0] != '*') {
      throw std::runtime_error("Invalid RESP array");
    }

    cursor++;
    remaining--;

    int num_elements = 0;
    while (remaining > 0 && std::isdigit(*cursor)) {
      num_elements = num_elements * 10 + (*cursor - '0');
      cursor++;
      remaining--;
    }

    if (remaining < 2 || cursor[0] != '\r' || cursor[1] != '\n')
      throw std::runtime_error("Invalid RESP array header");

    cursor += 2;
    remaining -= 2;

    if (num_elements <= 0)
      throw std::runtime_error("Empty RESP array");

    std::vector<BulkString> args;
    args.reserve(num_elements);

    for (int i = 0; i < num_elements; i++) {
      size_t consumed = 0;
      BulkString bs = BulkString::decode(cursor, remaining, consumed);

      args.push_back(std::move(bs));

      cursor += consumed;
      remaining -= consumed;
    }

    std::string command_name = args[0].get();

    std::vector<std::string> arguments;
    for (size_t i = 1; i < args.size(); i++) {
      arguments.push_back(args[i].get());
    }

    return router::get_command(command_name, arguments, server_, message_);
  }

  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        asio::buffer(data_),
        [this, self](std::error_code ec, std::size_t length) {
          if (!ec) {
            handle_request(std::string(data_.data(), length), this->server_);
          }
        });
  }

  void send_response(std::string resp) {
    auto self = shared_from_this();
    message_ = std::move(resp);
    asio::async_write(socket_, asio::buffer(message_),
                      [this, self](std::error_code ec, std::size_t) {
                        if (!ec) {
                          do_read();
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

    auto cmd = parse_command(req.c_str(), req.length());
    auto session_ptr = shared_from_this();

    cmd->serveRequest([session = session_ptr](std::string resp) {
      session->send_response(std::move(resp));
    });
  }

  void do_write() {
    auto self(shared_from_this());
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

  spdlog::info("Listening on port {}...", port_);
  spdlog::debug("Waiting for a client to connect...");
  accept();

  ctx_.run();

  return 0;
}
