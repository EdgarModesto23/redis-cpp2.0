#include "LpushCommand.hpp"
#include "integer.hpp"
#include <string>

LpushCommand::LpushCommand(std::shared_ptr<Database> db, std::string &buf,
                           std::string key, std::string element) noexcept
    : db_(db), buff_(buf), key_(key),
      element_(std::vector<std::string>{element}) {}

LpushCommand::LpushCommand(std::shared_ptr<Database> db, std::string &buf,
                           std::string key,
                           std::vector<std::string> element) noexcept
    : db_(db), buff_(buf), key_(key), element_(element) {}

void LpushCommand::serveRequest() {
  int size = db_->setListLeft(key_, element_);

  buff_ = Integer::to_resp(size);
}
