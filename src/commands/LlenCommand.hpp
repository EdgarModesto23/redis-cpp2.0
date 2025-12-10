#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class LlenCommand : public ICommand {
public:
  explicit LlenCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string key) noexcept;

  ~LlenCommand() override = default;

  LlenCommand(const LlenCommand &) = delete;
  LlenCommand &operator=(const LlenCommand &) = delete;
  LlenCommand(LlenCommand &&) noexcept = default;
  LlenCommand &operator=(LlenCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
};
