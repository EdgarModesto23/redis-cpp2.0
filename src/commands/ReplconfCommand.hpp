#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <string>

class ReplconfCommand : public ICommand {
public:
  explicit ReplconfCommand(std::shared_ptr<Database> db,
                           std::string &buf) noexcept;

  ~ReplconfCommand() override = default;

  ReplconfCommand(const ReplconfCommand &) = delete;
  ReplconfCommand &operator=(const ReplconfCommand &) = delete;
  ReplconfCommand(ReplconfCommand &&) noexcept = default;
  ReplconfCommand &operator=(ReplconfCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
};
