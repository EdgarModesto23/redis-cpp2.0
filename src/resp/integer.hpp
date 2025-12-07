#pragma once
#include <string>
#include <vector>

class Integer {
public:
  explicit Integer(long long value);

  char *encode();

  static Integer decode(const char *data, size_t size, size_t &consumed);

  static std::string to_resp(long long value);

  long long value() const { return value_; }

private:
  long long value_;
  std::vector<char> encodedBuffer_;
};
