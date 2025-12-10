#include "PingCommand.hpp"
#include <optional>
#include <unistd.h>

void PingCommand::serveRequest(std::function<void(std::string)> respond) {

  if (message_ != std::nullopt) {
    respond(message_.value());
    return;
  }

  respond("+PONG\r\n");
}
