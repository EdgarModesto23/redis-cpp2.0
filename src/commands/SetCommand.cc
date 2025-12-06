#include "SetCommand.hpp"
#include <string>

SetCommand::SetCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string key, std::string value,
                       std::optional<std::string> expiration) noexcept
    : db_(db), buff_(buf), key_(std::move(key)), value_(std::move(value)),
      expiration_(std::move(expiration)) {}

void SetCommand::serveRequest() {
  db_->setValue(key_, value_);
  buff_ = "+OK\r\n";
}
