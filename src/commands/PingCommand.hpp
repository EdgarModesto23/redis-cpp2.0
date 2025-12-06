#pragma once
#include "database.hpp"
#include <asio/ip/tcp.hpp>
#include <memory>
#include <optional>
#include <string>

#include "command.hpp"

class PingCommand : public ICommand {
public:
  explicit PingCommand(std::shared_ptr<Database> db, std::string &buf) noexcept
      : db_(db), buff_(buf), message_(std::nullopt) {};
  explicit PingCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string message) noexcept
      : db_(db), buff_(buf), message_(message) {};
  ~PingCommand() override = default;

  PingCommand(const PingCommand &) = delete;
  PingCommand &operator=(const PingCommand &) = delete;
  PingCommand(PingCommand &&) noexcept = default;
  PingCommand &operator=(PingCommand &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::optional<std::string> message_; // string
};
