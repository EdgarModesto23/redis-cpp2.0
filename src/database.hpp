#pragma once
#include <string>
#include <unordered_map>

class Database {
public:
  explicit Database() noexcept
      : kvStore(std::unordered_map<std::string, std::string>()) {};

  std::string getValue(std::string key);

private:
  std::unordered_map<std::string, std::string> kvStore;
};
