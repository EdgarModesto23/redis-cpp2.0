#pragma once
#include "bulkstring.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class RespArray;

class RespArray {
public:
  RespArray() = default;

  explicit RespArray(std::vector<BulkString> elements);
  explicit RespArray(std::vector<std::string> elements);

  char *encode();

  static RespArray decode(const char *data, size_t size, size_t &consumed);

  static std::string to_resp(const std::vector<std::string> &items);

  const std::vector<BulkString> &items() const { return elements_; }

private:
  std::vector<BulkString> elements_;
  std::vector<char> encodedBuffer_;
};
