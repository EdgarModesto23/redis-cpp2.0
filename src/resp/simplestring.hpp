#pragma once
#include "decode.hpp"
#include "encode.hpp"
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

// ---------- SimpleString Class ----------
class SimpleString : public IEncode {
public:
  SimpleString() = default;
  explicit SimpleString(const std::string &value);

  char *encode() override;

  static SimpleString decode(const char *data, size_t size, size_t &consumed);

  const std::string &get() const { return value_; }

private:
  std::string value_;
  std::vector<char> encodedBuffer_;
};
