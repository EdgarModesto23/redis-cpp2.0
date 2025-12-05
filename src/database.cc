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
