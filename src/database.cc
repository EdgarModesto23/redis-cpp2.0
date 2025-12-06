#include "database.hpp"
#include "asio.hpp"
#include <asio/error_code.hpp>
#include <chrono>
#include <cstdint>
#include <string>

std::string Database::getValue(std::string key) {
  auto it = kvStore_.find(key);

  if (it != kvStore_.end()) {
    return it->second;
  } else {
    return std::string();
  }
}

void Database::setValue(std::string key, std::string value, int64_t time) {
  kvStore_.insert({key, value});
  if (time > 0) {
    auto timer = std::make_shared<asio::steady_timer>(
        ctx_,
        std::chrono::steady_clock::now() + std::chrono::milliseconds(time));

    timer->async_wait([this, key, timer](const asio::error_code &ec) {
      if (!ec) {
        kvStore_.erase(key);
      }
    });
  }
}
