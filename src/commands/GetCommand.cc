#include "GetCommand.hpp"
#include "bulkstring.hpp"
#include <string>

GetCommand::GetCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string key) noexcept
    : db_(db), buff_(buf), key_(key) {}

void GetCommand::serveRequest(std::function<void(std::string)> respond) {
  auto val = db_->getValue(key_);

  if (val.empty()) {
    respond("$-1\r\n");
  } else {
    respond(BulkString::to_resp(val));
  }
}
