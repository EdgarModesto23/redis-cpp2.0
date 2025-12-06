#pragma once
#include <string>
#include <unordered_map>

class Database {
public:
  explicit Database() noexcept
      : kvStore(std::unordered_map<std::string, std::string>()) {};

  std::string getValue(std::string key);
  void setValue(std::string key, std::string value);

private:
  std::unordered_map<std::string, std::string> kvStore;
};
