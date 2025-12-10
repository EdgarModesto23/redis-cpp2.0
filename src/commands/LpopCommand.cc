#include "LpopCommand.hpp"
#include "array.hpp"
#include "bulkstring.hpp"
#include <string>

LpopCommand::LpopCommand(std::shared_ptr<Database> db, std::string &buf,
                         std::string key,
                         std::optional<long long> count) noexcept
    : db_(db), buff_(buf), key_(key), count_(count) {}

void LpopCommand::serveRequest() {
  if (!count_.has_value() || count_.value() <= 1) {
    buff_ = BulkString::to_resp(db_->removeListValue(key_));
  } else {
    buff_ = RespArray::to_resp(db_->removeListValue(key_, count_.value()));
  }
}
