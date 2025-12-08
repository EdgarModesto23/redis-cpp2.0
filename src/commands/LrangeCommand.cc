#include "LrangeCommand.hpp"
#include "array.hpp"
#include <exception>
#include <string>

LrangeCommand::LrangeCommand(std::shared_ptr<Database> db, std::string &buf,
                             std::string key, long long start,
                             long long stop) noexcept
    : db_(db), buff_(buf), key_(key), start_(start), stop_(stop) {}

void LrangeCommand::serveRequest() {
  buff_ = RespArray::to_resp(db_->getListRange(key_, start_, stop_));
}
