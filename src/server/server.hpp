#pragma once
#include "bulkstring.hpp"
#include "database.hpp"
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <memory>
#include <vector>

using asio::ip::tcp;

std::vector<IEncode> parse_request(const std::string &req, int length);

class Server {
public:
  Server(asio::io_context &ctx)
      : db_(std::make_shared<Database>(ctx)), ctx_(ctx),
        acceptor_(ctx_, tcp::endpoint(tcp::v4(), 6379)) {};

  int init_server();
  void accept();

  std::shared_ptr<Database> get_db() { return db_; }

private:
  std::shared_ptr<Database> db_;
  asio::io_context &ctx_;
  asio::ip::tcp::acceptor acceptor_;
};
