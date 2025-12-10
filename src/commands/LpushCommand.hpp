#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

class LpushCommand : public ICommand {
public:
  explicit LpushCommand(std::shared_ptr<Database> db, std::string &buf,
                        std::string key, std::string element) noexcept;

  explicit LpushCommand(std::shared_ptr<Database> db, std::string &buf,
                        std::string key,
                        std::vector<std::string> element) noexcept;

  ~LpushCommand() override = default;

  LpushCommand(const LpushCommand &) = delete;
  LpushCommand &operator=(const LpushCommand &) = delete;
  LpushCommand(LpushCommand &&) noexcept = default;
  LpushCommand &operator=(LpushCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  std::vector<std::string> element_;
};
