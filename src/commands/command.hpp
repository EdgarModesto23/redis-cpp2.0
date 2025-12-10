#pragma once

#include <functional>
#include <string>
class ICommand {
public:
  virtual void serveRequest(std::function<void(std::string)> respond) = 0;
  virtual ~ICommand() = default;
};
