#include "integer.hpp"
#include <cstring>
#include <stdexcept>

Integer::Integer(long long value) : value_(value) {}

char *Integer::encode() {
  std::string header = ":" + std::to_string(value_) + "\r\n";

  encodedBuffer_.resize(header.size());
  std::memcpy(encodedBuffer_.data(), header.data(), header.size());

  return encodedBuffer_.data();
}

Integer Integer::decode(const char *data, size_t size, size_t &consumed) {
  consumed = 0;

  if (size < 4 || data[0] != ':')
    throw std::runtime_error("Invalid Integer RESP (missing ':')");

  const char *numStart = data + 1;
  const char *lineEnd =
      static_cast<const char *>(std::memchr(numStart, '\r', size - 1));

  if (!lineEnd || lineEnd + 1 >= data + size || *(lineEnd + 1) != '\n')
    throw std::runtime_error("Invalid Integer RESP (missing CRLF)");

  long long number = 0;
  bool negative = false;
  const char *p = numStart;

  if (*p == '-') {
    negative = true;
    ++p;
    if (p == lineEnd)
      throw std::runtime_error("Invalid Integer: lone '-'");
  }

  for (; p < lineEnd; ++p) {
    if (!std::isdigit(*p))
      throw std::runtime_error("Invalid Integer: non-digit character");
    number = number * 10 + (*p - '0');
  }

  if (negative)
    number = -number;

  consumed = (lineEnd - data) + 2;

  return Integer(number);
}

std::string Integer::to_resp(long long value) {
  std::string resp;
  resp.reserve(32);

  resp += ":";
  resp += std::to_string(value);
  resp += "\r\n";

  return resp;
}
