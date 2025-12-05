#pragma once
class IEncode {
public:
  virtual char *encode() = 0;
  virtual ~IEncode() = default;
};
