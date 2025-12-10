#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class LpopCommand : public ICommand {
public:
  explicit LpopCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string key,
                       std::optional<long long> count) noexcept;

  ~LpopCommand() override = default;

  LpopCommand(const LpopCommand &) = delete;
  LpopCommand &operator=(const LpopCommand &) = delete;
  LpopCommand(LpopCommand &&) noexcept = default;
  LpopCommand &operator=(LpopCommand &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  std::optional<long long> count_;
};
