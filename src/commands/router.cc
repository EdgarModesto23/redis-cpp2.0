#include "router.hpp"
#include "EchoCommand.hpp"
#include "GetCommand.hpp"
#include "SetCommand.hpp"

enum class CommandType { PING, ECHO_CMD, SET, GET, UNKNOWN };

static CommandType to_command(const std::string &cmd) {
  if (cmd == "PING")
    return CommandType::PING;
  if (cmd == "ECHO")
    return CommandType::ECHO_CMD;
  if (cmd == "SET")
    return CommandType::SET;
  if (cmd == "GET")
    return CommandType::GET;
  return CommandType::UNKNOWN;
}

std::unique_ptr<ICommand>
router::get_command(const std::string &cmd_name,
                    const std::vector<std::string> &args, Server &srv,
                    std::string &buffer) {
  auto db = srv.get_db();

  switch (to_command(cmd_name)) {

  case CommandType::PING:
    if (!args.empty()) {
      return std::make_unique<PingCommand>(db, buffer, args[0]);
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::ECHO_CMD:
    if (!args.empty()) {
      return std::make_unique<EchoCommand>(db, buffer, args[0]);
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::GET:
    if (!args.empty()) {
      return std::make_unique<GetCommand>(db, buffer, args[0]);
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::SET:
    if (!args.empty()) {
      return std::make_unique<SetCommand>(db, buffer, args[0], args[1],
                                          std::nullopt);
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::UNKNOWN:
  default:
    return std::make_unique<PingCommand>(db, buffer);
  }
}
