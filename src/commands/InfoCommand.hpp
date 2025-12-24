#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class InfoCommand : public ICommand {
public:
  explicit InfoCommand(std::shared_ptr<Database> db, std::string &buf,
                       std::optional<std::string> section) noexcept;

  ~InfoCommand() override = default;

  InfoCommand(const InfoCommand &) = delete;
  InfoCommand &operator=(const InfoCommand &) = delete;
  InfoCommand(InfoCommand &&) noexcept = default;
  InfoCommand &operator=(InfoCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::optional<std::string> section_;
};
