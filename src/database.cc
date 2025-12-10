#include "database.hpp"
#include "asio.hpp"
#include <algorithm>
#include <asio/error_code.hpp>
#include <chrono>
#include <cstdint>
#include <exception>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

class KeyDoesnotExist : public std::exception {
public:
  explicit KeyDoesnotExist(const char *key) : key_(key) {
    // Build a descriptive error message
    message_ = "Key does not exist: ";
    message_ += key_;
  }

  const char *what() const noexcept override { return message_.c_str(); }

private:
  const char *key_;
  std::string message_;
};

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

std::string Database::removeListValue(const std::string &key) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return std::string();
  }

  auto &vec = listStore_[key];

  auto val = vec.front();
  vec.erase(vec.begin());

  return val;
}

std::vector<std::string> Database::removeListValue(const std::string &key,
                                                   int idx) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return std::vector<std::string>{};
  }

  std::vector<std::string> values{};

  auto &vec = listStore_[key];

  // This is trash and i'm aware of it
  for (int i = 0; i < idx; i++) {
    auto val = vec.front();
    vec.erase(vec.begin());
    values.push_back(std::move(val));
  }

  return values;
}

std::vector<std::string> Database::getListRange(const std::string &key,
                                                int begin, int end) {
  auto listOpt = findlistStoreValue(key);

  if (!listOpt) {
    return {};
  }

  const auto &vec = *listOpt;
  int size = static_cast<int>(vec.size());

  if (begin < 0)
    begin = size + begin;
  if (end < 0)
    end = size + end;

  if (begin < 0)
    begin = 0;
  if (end >= size)
    end = size - 1;

  if (begin > end || begin >= size) {
    return {};
  }

  return {vec.begin() + begin, vec.begin() + end + 1};
}

int Database::setListLeft(const std::string &key,
                          std::vector<std::string> &value) {

  auto &vec = listStore_[key];
  vec.reserve(vec.size() + value.size());

  for (auto &v : value)
    vec.push_back(v);

  std::reverse(vec.begin(), vec.end());

  return vec.size();
}

void Database::asyncBlpop(const std::string &key, double timeout,
                          BlpopHandler handler) {
  // 1. Try immediate pop:
  auto listOpt = findlistStoreValue(key);
  if (listOpt && !listOpt->empty()) {
    handler(removeListValue(key));
    return;
  }

  size_t id = waiter_count_++;

  // Create a Waiter (timer may be null if timeout==0)
  std::shared_ptr<asio::steady_timer> timer = nullptr;

  // 2. If timeout > 0, create timer. If timeout == 0 → wait indefinitely.
  if (timeout > 0) {
    timer = std::make_shared<asio::steady_timer>(ctx_);
    timer->expires_after(
        std::chrono::milliseconds(static_cast<int>(timeout * 1000)));

    timer->async_wait([this, key, id](const asio::error_code &ec) {
      if (ec)
        return; // timer canceled by wake-up event

      auto &vec = waiters_[key];

      auto it = std::find_if(vec.begin(), vec.end(),
                             [id](const Waiter &w) { return w.id == id; });

      if (it != vec.end()) {
        auto handler = it->handler;
        vec.erase(it);
        handler(std::nullopt); // timeout
      }
    });
  }

  // 3. Add waiter to queue
  waiters_[key].push_back(Waiter{id, handler, timer});
}

void Database::notifyListPush(const std::string &key) {
  auto it = waiters_.find(key);
  if (it == waiters_.end()) {
    spdlog::error("Not found");
    return;
  }

  auto &queue = it->second;
  if (queue.empty()) {
    spdlog::error("Queue empty");
    return;
  }

  // Pop oldest waiter
  Waiter w = queue.front();
  queue.erase(queue.begin());

  // Cancel timeout timer (if any)

  if (w.timer) {
    w.timer->cancel();
  }

  // Deliver value
  w.handler(removeListValue(key));
}
