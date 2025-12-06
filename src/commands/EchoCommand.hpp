#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class EchoCommand : public ICommand {
public:
  explicit EchoCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string message) noexcept;

  ~EchoCommand() override = default;

  EchoCommand(const EchoCommand &) = delete;
  EchoCommand &operator=(const EchoCommand &) = delete;
  EchoCommand(EchoCommand &&) noexcept = default;
  EchoCommand &operator=(EchoCommand &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string message_;
};
