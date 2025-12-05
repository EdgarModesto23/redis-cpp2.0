#pragma once

#include "PingCommand.hpp"
#include "bulkstring.hpp"
#include "server.hpp"
#include <memory>

namespace router {
std::unique_ptr<ICommand> get_command(BulkString s, Server &srv_,
                                      std::string &buffer);

}
