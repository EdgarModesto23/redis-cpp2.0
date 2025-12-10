#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class LrangeCommand : public ICommand {
public:
  explicit LrangeCommand(std::shared_ptr<Database> db, std::string &buf,
                         std::string key, long long start,
                         long long stop) noexcept;

  ~LrangeCommand() override = default;

  LrangeCommand(const LrangeCommand &) = delete;
  LrangeCommand &operator=(const LrangeCommand &) = delete;
  LrangeCommand(LrangeCommand &&) noexcept = default;
  LrangeCommand &operator=(LrangeCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  long long start_;
  long long stop_;
};
