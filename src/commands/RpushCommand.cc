#include "RpushCommand.hpp"
#include "integer.hpp"
#include <functional>
#include <string>
#include <vector>

RpushCommand::RpushCommand(std::shared_ptr<Database> db, std::string &buf,
                           std::string key, std::string element) noexcept
    : db_(db), buff_(buf), key_(key),
      element_(std::vector<std::string>({element})) {}

RpushCommand::RpushCommand(std::shared_ptr<Database> db, std::string &buf,
                           std::string key,
                           std::vector<std::string> element) noexcept
    : db_(db), buff_(buf), key_(key), element_(element) {}

void RpushCommand::serveRequest(std::function<void(std::string)> respond) {
  int size = db_->setList(key_, element_);

  respond(Integer::to_resp(size));
  db_->notifyListPush(key_);
}
