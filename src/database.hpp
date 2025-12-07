#pragma once
#include <asio/io_context.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

class Database {
public:
  explicit Database(asio::io_context &ctx) noexcept
      : kvStore_(std::unordered_map<std::string, std::string>()),
        listStore_(std::unordered_map<std::string, std::vector<std::string>>()),
        ctx_(ctx) {};

  std::string getValue(const std::string &key);
  void setValue(const std::string &key, std::string &value, int64_t time = 0);

  int setList(const std::string &key, std::string &value);
  int setList(const std::string &key, std::vector<std::string> value);
  std::string getListValue(const std::string &key, int idx = -1);
  size_t getListLength(const std::string &);
  void removeListValue(const std::string &key, int idx);
  void removeListValue(const std::string &key, std::vector<int> idx);

private:
  std::unordered_map<std::string, std::string> kvStore_;
  std::unordered_map<std::string, std::vector<std::string>> listStore_;
  asio::io_context &ctx_;

  std::optional<std::string> findKvStoreValue(const std::string &key);
  std::optional<std::vector<std::string>>
  findlistStoreValue(const std::string &key);
};
