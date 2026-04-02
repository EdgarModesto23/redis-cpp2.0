#pragma once
#include "bulkstring.hpp"
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

class RespArray;

class RespArray {
public:
  struct Arg {
    std::string text;

    Arg(const char *s) : text(s) {}
    Arg(const std::string &s) : text(s) {}
    Arg(std::string &&s) : text(std::move(s)) {}
    Arg(std::string_view sv) : text(sv) {}
    Arg(const BulkString &bs) : text(bs.get()) {}

    template <typename T>
      requires std::is_arithmetic_v<std::remove_reference_t<T>>
    Arg(T n) : text(std::to_string(n)) {}
  };

  RespArray() = default;

  explicit RespArray(std::vector<BulkString> elements);
  explicit RespArray(std::vector<std::string> elements);
  RespArray(std::initializer_list<Arg> init);

  char *encode();

  static RespArray decode(const char *data, size_t size, size_t &consumed);

  static std::string to_resp(const std::vector<std::string> &items);

  const std::vector<BulkString> &items() const { return elements_; }

private:
  std::vector<BulkString> elements_;
  std::vector<char> encodedBuffer_;
};
