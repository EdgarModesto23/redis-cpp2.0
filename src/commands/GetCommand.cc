#include "GetCommand.hpp"
#include "bulkstring.hpp"
#include <string>

GetCommand::GetCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string key) noexcept
    : db_(db), buff_(buf), key_(key) {}

void GetCommand::serveRequest() {
  auto val = db_->getValue(key_);

  if (val.empty()) {
    buff_ = "$-1\r\n";
  } else {
    buff_ = BulkString::to_resp(val);
  }
}
