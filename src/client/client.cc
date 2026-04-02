#include "client.hpp"
#include "bulkstring.hpp"
#include "simplestring.hpp"
#include <asio.hpp>
#include <memory>

void Client::connect() {
  asio::ip::tcp::resolver resolver(ctx_);
  auto endpoints = resolver.resolve(host_, port_);

  socket_ = std::make_unique<asio::ip::tcp::socket>(ctx_);
  asio::connect(*socket_, endpoints);
}

SimpleString Client::ping() {
  std::string request = "*1\r\n$4\r\nPING\r\n";

  asio::write(*socket_, asio::buffer(request));
  std::vector<char> buf(4096);
  const size_t n =
      asio::read(*socket_, asio::buffer(buf), asio::transfer_at_least(1));
  size_t consumed = 0;
  return SimpleString::decode(buf.data(), n, consumed);
}
