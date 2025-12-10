#pragma once
#include <memory>
#include <optional>
#include <string>
#include "database.hpp"
#include "command.hpp"

class LlenCommand : public ICommand {
public:
  explicit LlenCommand(std::shared_ptr<Database> db, std::string &buf, std::string key) noexcept;

  ~LlenCommand() override = default;

  LlenCommand(const LlenCommand &) = delete;
  LlenCommand &operator=(const LlenCommand &) = delete;
  LlenCommand(LlenCommand &&) noexcept = default;
  LlenCommand &operator=(LlenCommand &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;

};
