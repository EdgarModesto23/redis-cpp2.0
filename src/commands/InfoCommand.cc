#include "InfoCommand.hpp"
#include "bulkstring.hpp"
#include <string>

InfoCommand::InfoCommand(std::shared_ptr<Database> db, std::string &buf,
                         std::optional<std::string> section) noexcept
    : db_(db), buff_(buf), section_(section) {}

void InfoCommand::serveRequest(std::function<void(std::string)> respond) {
  if (db_->config_.master_address == "master")
    respond(BulkString::to_resp(
        "role:master\nmastereplid:"
        "8371b4fb1155b71f4a04d3e1bc3e18c4a990aeeb\nmaster_repl_offset:0"));
  else
    respond(BulkString::to_resp(
        "role:slave\nmastereplid:"
        "8371b4fb1155b71f4a04d3e1bc3e18c4a990aeeb\nmaster_repl_"
        "offset:0"));
}
