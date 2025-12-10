#pragma once
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

class Database {
public:
  explicit Database(asio::io_context &ctx) noexcept
      : kvStore_(std::unordered_map<std::string, std::string>()),
        listStore_(std::unordered_map<std::string, std::vector<std::string>>()),
        ctx_(ctx),
        waiters_(std::unordered_map<std::string, std::vector<Waiter>>()),
        waiter_count_(0) {};

  std::string getValue(const std::string &key);
  void setValue(const std::string &key, std::string &value, int64_t time = 0);

  int setListLeft(const std::string &key, std::vector<std::string> &value);
  int setList(const std::string &key, std::string &value);
  int setList(const std::string &key, std::vector<std::string> value);
  std::string getListValue(const std::string &key, int idx = -1);
  std::vector<std::string> getListRange(const std::string &key, int begin,
                                        int end = -1);
  size_t getListLength(const std::string &key);
  std::string removeListValue(const std::string &key);
  std::vector<std::string> removeListValue(const std::string &key, int idx);
  using BlpopHandler = std::function<void(std::optional<std::string>)>;

  struct Waiter {
    size_t id;
    BlpopHandler handler;
    std::shared_ptr<asio::steady_timer> timer;
  };

  void asyncBlpop(const std::string &key, double timeout, BlpopHandler handler);

  void notifyListPush(const std::string &key);

private:
  std::unordered_map<std::string, std::string> kvStore_;
  std::unordered_map<std::string, std::vector<std::string>> listStore_;

  std::unordered_map<std::string, std::vector<Waiter>> waiters_;
  size_t waiter_count_;
  asio::io_context &ctx_;

  std::optional<std::string> findKvStoreValue(const std::string &key);
  std::optional<std::vector<std::string>>
  findlistStoreValue(const std::string &key);
};
