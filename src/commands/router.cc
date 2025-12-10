#include "router.hpp"
#include "EchoCommand.hpp"
#include "GetCommand.hpp"
#include "LlenCommand.hpp"
#include "LpushCommand.hpp"
#include "LrangeCommand.hpp"
#include "RpushCommand.hpp"
#include "SetCommand.hpp"
#include <cstdlib>
#include <memory>
#include <string>

enum class CommandType {
  PING,
  ECHO_CMD,
  SET,
  GET,
  RPUSH,
  LPUSH,
  LRANGE,
  LLEN,
  UNKNOWN
};

static CommandType to_command(const std::string &cmd) {
  if (cmd == "PING")
    return CommandType::PING;
  if (cmd == "ECHO")
    return CommandType::ECHO_CMD;
  if (cmd == "SET")
    return CommandType::SET;
  if (cmd == "GET")
    return CommandType::GET;
  if (cmd == "RPUSH")
    return CommandType::RPUSH;
  if (cmd == "LRANGE")
    return CommandType::LRANGE;
  if (cmd == "LPUSH")
    return CommandType::LPUSH;
  if (cmd == "LLEN")
    return CommandType::LLEN;
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
      if (args.size() >= 4) {
        if (args[2].compare("PX") == 0 || args[2].compare("EX") == 0) {
          return std::make_unique<SetCommand>(db, buffer, args[0], args[1],
                                              args[3]);
        }
      }
      return std::make_unique<SetCommand>(db, buffer, args[0], args[1],
                                          std::nullopt);
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::RPUSH:
    if (!args.empty()) {
      return std::make_unique<RpushCommand>(
          db, buffer, args[0],
          std::vector<std::string>(args.begin() + 1, args.end()));
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::LRANGE:
    if (!args.empty()) {
      return std::make_unique<LrangeCommand>(
          db, buffer, args[0], std::stoll(args[1]), std::stoll(args[2]));
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::LPUSH:
    if (!args.empty()) {
      return std::make_unique<LpushCommand>(
          db, buffer, args[0],
          std::vector<std::string>(args.begin() + 1, args.end()));
    }

  case CommandType::LLEN:
    if (!args.empty()) {
      return std::make_unique<LlenCommand>(db, buffer, args[0]);
    }
    return std::make_unique<PingCommand>(db, buffer);

  case CommandType::UNKNOWN:
  default:
    return std::make_unique<PingCommand>(db, buffer);
  }
}
