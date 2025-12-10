#include "BlpopCommand.hpp"
#include "array.hpp"
#include <spdlog/spdlog.h>
#include <string>

BlpopCommand::BlpopCommand(std::shared_ptr<Database> db, std::string &buf,
                           std::string key, double timeout) noexcept
    : db_(db), buff_(buf), key_(key), timeout_(timeout) {}

void BlpopCommand::serveRequest(std::function<void(std::string)> respond) {
  db_->asyncBlpop(key_, timeout_,
                  [respond, key = key_](std::optional<std::string> result) {
                    if (!result.has_value()) {
                      respond(RespArray::to_resp({}));
                    } else {
                      respond(RespArray::to_resp({key, result.value()}));
                    }
                  });
}
