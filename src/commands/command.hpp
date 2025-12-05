#pragma once

class ICommand {
public:
  virtual void serveRequest() = 0;
  virtual ~ICommand() = default;
};
