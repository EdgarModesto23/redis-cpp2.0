#include "PingCommand.hpp"
#include <optional>
#include <unistd.h>

void PingCommand::serveRequest() {

  if (message_ != std::nullopt) {
    buff_ = message_.value();
    return;
  }

  buff_ = "+PONG\r\n";
}
