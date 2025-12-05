#include "bulkstring.hpp"
#include <cstdio>
#include <spdlog/spdlog.h>

// ---------------- Constructor ----------------
BulkString::BulkString(const std::string &value) : value_(value) {}

// ---------------- encode() ----------------
char *BulkString::encode() {
  // RESP format: $<length>\r\n<data>\r\n
  std::string header = "$" + std::to_string(value_.size()) + "\r\n";

  size_t totalSize = header.size() + value_.size() + 2;
  encodedBuffer_.resize(totalSize);

  size_t offset = 0;
  std::memcpy(encodedBuffer_.data(), header.data(), header.size());
  offset += header.size();

  std::memcpy(encodedBuffer_.data() + offset, value_.data(), value_.size());
  offset += value_.size();

  encodedBuffer_[offset++] = '\r';
  encodedBuffer_[offset++] = '\n';

  return encodedBuffer_.data();
}

// ---------------- decode() ----------------
BulkString BulkString::decode(const char *data, size_t size, size_t &consumed) {
  consumed = 0;

  if (size < 4 || data[0] != '$')
    throw std::runtime_error("Invalid BulkString");

  const char *lenStart = data + 1;
  const char *lenEnd =
      static_cast<const char *>(std::memchr(lenStart, '\r', size - 1));

  if (!lenEnd || lenEnd + 1 >= data + size || *(lenEnd + 1) != '\n')
    throw std::runtime_error("Invalid BulkString length header");

  int len = 0;
  for (const char *p = lenStart; p < lenEnd; ++p) {
    if (!std::isdigit(*p))
      throw std::runtime_error("Non-numeric length");
    len = len * 10 + (*p - '0');
  }

  const char *contentStart = lenEnd + 2;

  if (contentStart + len + 2 > data + size)
    throw std::runtime_error("BulkString content out of bounds");

  // create value
  std::string value(contentStart, len);

  consumed = (contentStart - data) + len + 2;

  return BulkString(value);
}
