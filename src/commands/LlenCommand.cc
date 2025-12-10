#include "LlenCommand.hpp"
#include "integer.hpp"
#include <string>

LlenCommand::LlenCommand(std::shared_ptr<Database> db, std::string &buf,
                         std::string key) noexcept
    : db_(db), buff_(buf), key_(key) {}

void LlenCommand::serveRequest() {
  buff_ = Integer::to_resp(db_->getListLength(key_));
}
