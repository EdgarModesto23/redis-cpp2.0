#include "SetCommand.hpp"
#include <optional>
#include <string>

SetCommand::SetCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::string key, std::string value,
                       std::optional<std::string> expiration) noexcept
    : db_(db), buff_(buf), key_(std::move(key)), value_(std::move(value)),
      expiration_(std::move(expiration)) {}

void SetCommand::serveRequest(std::function<void(std::string)> respond) {
  if (expiration_ != std::nullopt) {
    db_->setValue(key_, value_, std::stoll(expiration_.value()));
  } else {
    db_->setValue(key_, value_);
  }
  respond("+OK\r\n");
}
