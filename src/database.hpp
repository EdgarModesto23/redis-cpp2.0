#pragma once
#include <asio/io_context.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

class Database {
public:
  explicit Database(asio::io_context &ctx) noexcept
      : kvStore_(std::unordered_map<std::string, std::string>()), ctx_(ctx) {};

  std::string getValue(std::string key);
  void setValue(std::string key, std::string value, int64_t time = 0);

private:
  std::unordered_map<std::string, std::string> kvStore_;
  asio::io_context &ctx_;
};
