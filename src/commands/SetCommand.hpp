#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class SetCommand : public ICommand {
public:
  explicit SetCommand(std::shared_ptr<Database> db, std::string &buf,
                      std::string key, std::string value,
                      std::optional<std::string> expiration) noexcept;

  ~SetCommand() override = default;

  SetCommand(const SetCommand &) = delete;
  SetCommand &operator=(const SetCommand &) = delete;
  SetCommand(SetCommand &&) noexcept = default;
  SetCommand &operator=(SetCommand &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  std::string value_;
  std::optional<std::string> condition_;
  std::optional<std::string> get_;
  std::optional<std::string> expiration_;
};
