#pragma once
#include "Config.h"

class LineReceiver {
 public:
  enum class Result { None, Line, Rejected };
  Result consume(int byte);
  const char* line() const { return buffer_; }
  // Only meaningful immediately after Result::Rejected.
  const char* reason() const { return rejection_; }

 private:
  char buffer_[config::lineCapacity] = {};
  unsigned int length_ = 0;
  bool discard_ = false;
  bool previousCR_ = false;
  const char* rejection_ = "none";
};
