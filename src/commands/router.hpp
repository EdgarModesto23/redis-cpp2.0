#pragma once

#include "PingCommand.hpp"
#include "bulkstring.hpp"
#include "server.hpp"
#include <memory>

namespace router {
std::unique_ptr<ICommand> get_command(std::string &cmd_name,
                                      const std::vector<std::string> &args,
                                      Server &srv, std::string &buffer);

}
