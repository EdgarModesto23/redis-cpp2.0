#pragma once
#include "array.hpp"
#include "bulkstring.hpp"
#include "simplestring.hpp"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/write.hpp>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

class Client {
public:
  Client(const std::string &host, const std::string &port,
         asio::io_context &ctx)
      : host_(host), port_(port), ctx_(ctx) {}

  void connect();

  SimpleString ping();

  template <typename T> T send_array_command(RespArray command);

private:
  std::string host_;
  std::string port_;
  asio::io_context &ctx_;
  std::unique_ptr<asio::ip::tcp::socket> socket_;
};

template <typename T> T Client::send_array_command(RespArray command) {
  char *encoded = command.encode();
  const size_t len = std::strlen(encoded);
  asio::write(*socket_, asio::buffer(encoded, len));

  std::vector<char> buf(4096);
  const size_t n =
      asio::read(*socket_, asio::buffer(buf), asio::transfer_at_least(1));
  buf.shrink_to_fit();
  size_t consumed = 0;
  return T::decode(buf.data(), n, consumed);
}
