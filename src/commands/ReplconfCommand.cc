#include "ReplconfCommand.hpp"
#include <string>

ReplconfCommand::ReplconfCommand(std::shared_ptr<Database> db,
                                 std::string &buf) noexcept
    : db_(db), buff_(buf) {}

void ReplconfCommand::serveRequest(std::function<void(std::string)> respond) {

  respond("+OK\r\n");
}
