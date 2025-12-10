#include "EchoCommand.hpp"
#include "bulkstring.hpp"
#include <string>

EchoCommand::EchoCommand(std::shared_ptr<Database> db, std::string &buf,
                         std::string message) noexcept
    : db_(db), buff_(buf), message_(message) {}

void EchoCommand::serveRequest(std::function<void(std::string)> respond) {
  respond(BulkString::to_resp(message_));
}
