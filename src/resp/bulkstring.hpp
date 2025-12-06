#pragma once
#include "decode.hpp"
#include "encode.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class BulkString : public IEncode {
public:
  BulkString() = default;
  explicit BulkString(const std::string &value);

  char *encode() override;

  static BulkString decode(const char *data, size_t size, size_t &consumed);
  static std::string to_resp(const std::string &val);

  const std::string &get() const { return value_; }

private:
  std::string value_;
  std::vector<char> encodedBuffer_;
};
