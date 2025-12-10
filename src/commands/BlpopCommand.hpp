#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class BlpopCommand : public ICommand {
public:
  explicit BlpopCommand(std::shared_ptr<Database> db, std::string &buf,
                        std::string key, double timeout) noexcept;

  ~BlpopCommand() override = default;

  BlpopCommand(const BlpopCommand &) = delete;
  BlpopCommand &operator=(const BlpopCommand &) = delete;
  BlpopCommand(BlpopCommand &&) noexcept = default;
  BlpopCommand &operator=(BlpopCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
  double timeout_;
};
