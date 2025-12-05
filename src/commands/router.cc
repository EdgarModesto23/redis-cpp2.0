#include "router.hpp"

enum class CommandType { PING, UNKNOWN };

CommandType to_command(const BulkString &s) {
  const std::string &v = s.get();

  if (v == "PING")
    return CommandType::PING;

  return CommandType::UNKNOWN;
}

std::unique_ptr<ICommand> router::get_command(BulkString s, Server &srv_,
                                              std::string &buffer) {
  switch (to_command(s)) {
  case CommandType::PING:
    return std::make_unique<PingCommand>(srv_.get_db(), buffer);
  case CommandType::UNKNOWN:
    return std::make_unique<PingCommand>(srv_.get_db(), buffer);

  default:
    return std::make_unique<PingCommand>(srv_.get_db(), buffer);
  };
}
