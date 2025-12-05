#include <exception>
#include <string>

class WrongFormatException : public std::exception {
private:
  std::string message;

public:
  // Constructor to set a custom message
  WrongFormatException(const std::string &msg) : message(msg) {}

  // Override the what() method to return the custom message
  const char *what() const noexcept override { return message.c_str(); }
};
