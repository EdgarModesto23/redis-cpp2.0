#include "database.hpp"
#include <string>

std::string Database::getValue(std::string key) {
  auto it = kvStore.find(key);

  if (it != kvStore.end()) {
    return it->second;
  } else {
    return std::string();
  }
}

void Database::setValue(std::string key, std::string value) {
  kvStore.insert({key, value});
}
