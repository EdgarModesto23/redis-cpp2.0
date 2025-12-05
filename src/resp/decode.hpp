#pragma once
#include <concepts>
#include <cstddef>

template <typename T>
concept Decodable = requires(char *data, size_t size, size_t &consumed) {
  { T::decode(data, size) } -> std::same_as<T>;
};

template <typename T>
  requires Decodable<T>
T decodeFromRaw(char *data, size_t size) {
  return T::decode(data, size);
};
