#include "simplestring.hpp"
#include <cstdio>

// ---------------- Constructor ----------------
SimpleString::SimpleString(const std::string &value) : value_(value) {}

// ---------------- encode() ----------------
char *SimpleString::encode() {
  size_t totalSize = 1 + value_.size() + 2;
  encodedBuffer_.resize(totalSize);

  size_t offset = 0;
  encodedBuffer_[offset++] = '+';

  std::memcpy(encodedBuffer_.data() + offset, value_.data(), value_.size());
  offset += value_.size();

  encodedBuffer_[offset++] = '\r';
  encodedBuffer_[offset++] = '\n';

  return encodedBuffer_.data();
}

// ---------------- decode() ----------------
SimpleString SimpleString::decode(const char *data, size_t size,
                                  size_t &consumed) {
  consumed = 0;
  if (size < 3 || data[0] != '+') {
    throw std::runtime_error("Invalid SimpleString: Missing '+'");
  }

  const char *end = static_cast<const char *>(std::memchr(data, '\r', size));
  if (!end || end + 1 >= data + size || *(end + 1) != '\n') {
    throw std::runtime_error("Invalid SimpleString: Missing CRLF");
  }

  std::string content(data + 1, end - (data + 1));

  consumed = (end - data) + 2;

  return SimpleString(content);
}
