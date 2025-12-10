#pragma once
#include "command.hpp"
#include "database.hpp"
#include <memory>
#include <optional>
#include <string>

class GetCommand : public ICommand {
public:
  explicit GetCommand(std::shared_ptr<Database> db, std::string &buf,
                      std::string key) noexcept;

  ~GetCommand() override = default;

  GetCommand(const GetCommand &) = delete;
  GetCommand &operator=(const GetCommand &) = delete;
  GetCommand(GetCommand &&) noexcept = default;
  GetCommand &operator=(GetCommand &&) noexcept = delete;

  void serveRequest(std::function<void(std::string)> respond) override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
  std::string key_;
};
