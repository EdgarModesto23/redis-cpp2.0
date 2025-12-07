#include "database.hpp"
#include "asio.hpp"
#include <asio/error_code.hpp>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

std::optional<std::string> Database::findKvStoreValue(const std::string &key) {
  auto it = kvStore_.find(key);

  if (it != kvStore_.end()) {
    return it->second;
  } else {
    return std::nullopt;
  }
}

std::optional<std::vector<std::string>>
Database::findlistStoreValue(const std::string &key) {
  auto it = listStore_.find(key);

  if (it != listStore_.end()) {
    return it->second;
  } else {
    return std::nullopt;
  }
}

std::string Database::getValue(const std::string &key) {
  auto val = findKvStoreValue(key);
  if (val == std::nullopt) {
    return std::string();
  } else {
    return val.value();
  }
}

void Database::setValue(const std::string &key, std::string &value,
                        int64_t time) {
  kvStore_.insert({key, value});
  if (time > 0) {
    auto timer = std::make_shared<asio::steady_timer>(
        ctx_,
        std::chrono::steady_clock::now() + std::chrono::milliseconds(time));

    timer->async_wait([this, key, timer](const asio::error_code &ec) {
      if (!ec) {
        kvStore_.erase(key);
      }
    });
  }
}

int Database::setList(const std::string &key, std::string &value) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    listStore_[key] = {std::move(value)};
    return 1;
  }

  auto &vec = listStore_[key];
  vec.push_back(std::move(value));
  return vec.size();
}

int Database::setList(const std::string &key, std::vector<std::string> value) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    listStore_[key] = {std::move(value)};
    return listStore_[key].size();
  }

  auto &vec = listStore_[key];
  for (auto val : value) {
    vec.push_back(std::move(val));
  }
  return vec.size();
}

std::string Database::getListValue(const std::string &key, int idx) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return std::string();
  }

  auto &vec = listStore_[key];
  if (idx == -1) {
    return vec.back();
  }
  return vec.at(idx);
}

size_t Database::getListLength(const std::string &key) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return 0;
  }

  auto &vec = listStore_[key];

  return vec.size();
}
void Database::removeListValue(const std::string &key, int idx) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return;
  }

  auto &vec = listStore_[key];
  if (idx == -1) {
    vec.pop_back();
  }

  vec.erase(vec.begin() + idx);

  return;
}

void Database::removeListValue(const std::string &key, std::vector<int> idx) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return;
  }

  auto &vec = listStore_[key];
  if (idx.empty()) {
    return;
  }

  for (auto i : idx) {
    vec.erase(vec.begin() + i);
  }

  return;
}
