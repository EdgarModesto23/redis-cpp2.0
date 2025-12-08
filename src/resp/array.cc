#include "array.hpp"
#include <cctype>
#include <cstdio>
#include <cstring>

RespArray::RespArray(std::vector<BulkString> elements)
    : elements_(std::move(elements)) {}

char *RespArray::encode() {
  std::string header = "*" + std::to_string(elements_.size()) + "\r\n";

  size_t total = header.size();
  for (auto &el : elements_) {
    BulkString temp(el);
    char *encoded = temp.encode();
    total += strlen(encoded);
  }

  encodedBuffer_.clear();
  encodedBuffer_.resize(total);

  size_t offset = 0;
  std::memcpy(encodedBuffer_.data(), header.data(), header.size());
  offset += header.size();

  for (auto &el : elements_) {
    BulkString temp(el);
    char *encoded = temp.encode();
    size_t len = strlen(encoded);
    std::memcpy(encodedBuffer_.data() + offset, encoded, len);
    offset += len;
  }

  return encodedBuffer_.data();
}

RespArray RespArray::decode(const char *data, size_t size, size_t &consumed) {
  consumed = 0;
  if (size < 3 || data[0] != '*')
    throw std::runtime_error("Invalid RESP Array");

  const char *lenStart = data + 1;
  const char *lenEnd =
      static_cast<const char *>(std::memchr(lenStart, '\r', size - 1));
  if (!lenEnd || *(lenEnd + 1) != '\n')
    throw std::runtime_error("Invalid RESP Array header");

  int num = 0;
  for (const char *p = lenStart; p < lenEnd; ++p) {
    if (!std::isdigit(*p))
      throw std::runtime_error("Invalid array length");
    num = num * 10 + (*p - '0');
  }

  const char *cursor = lenEnd + 2;
  size_t remaining = size - (cursor - data);

  std::vector<BulkString> elements;
  elements.reserve(num);

  for (int i = 0; i < num; i++) {
    size_t consumedBS = 0;
    BulkString bs = BulkString::decode(cursor, remaining, consumedBS);

    elements.push_back(std::move(bs));

    cursor += consumedBS;
    remaining -= consumedBS;
  }

  consumed = cursor - data;
  return RespArray(std::move(elements));
}

std::string RespArray::to_resp(const std::vector<std::string> &items) {
  std::string out;
  out.reserve(32 + items.size() * 16);

  out += "*";
  out += std::to_string(items.size());
  out += "\r\n";

  for (auto &s : items) {
    out += BulkString::to_resp(s);
  }

  return out;
}
