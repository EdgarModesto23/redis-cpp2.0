#pragma once
#include "command.hpp"
#include "database.hpp"
#include <functional>
#include <memory>
#include <optional>
#include <string>

class RpushCommand : public ICommand {
public:
  explicit RpushCommand(std::shared_ptr<Database> db, std::string &buf,
                        std::string key,
                        std::vector<std::string> elements) noexcept;

  explicit RpushCommand(std::shared_ptr<Database> db, std::string &buf,
                        std::string key, std::string element) noexcept;

  ~RpushCommand() override = default;

  RpushCommand(const RpushCommand &) = delete;
  RpushCommand &operator=(const RpushCommand &) = delete;
  RpushCommand(RpushCommand &&) noexcept = default;
  RpushCommand &operator=(RpushCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  std::vector<std::string> element_;
};
