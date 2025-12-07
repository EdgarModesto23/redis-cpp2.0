#pragma once
#include <memory>
#include <optional>
#include <string>
#include "database.hpp"
#include "command.hpp"

class RpushCommand : public ICommand {
public:
  explicit RpushCommand(std::shared_ptr<Database> db, std::string &buf, std::string key, std::string element) noexcept;

  ~RpushCommand() override = default;

  RpushCommand(const RpushCommand &) = delete;
  RpushCommand &operator=(const RpushCommand &) = delete;
  RpushCommand(RpushCommand &&) noexcept = default;
  RpushCommand &operator=(RpushCommand &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  std::string element_;

};
