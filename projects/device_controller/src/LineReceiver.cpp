#include "LineReceiver.h"

LineReceiver::Result LineReceiver::consume(int byte) {
  if (byte < 0 || byte > 255) { return Result::None; }
  if (byte == '\n' && previousCR_) {
    previousCR_ = false;
    return Result::None;
  }
  previousCR_ = byte == '\r';
  if (byte == '\r' || byte == '\n') {
    const bool rejected = discard_;
    const bool nonempty = length_ > 0;
    buffer_[length_] = '\0';
    length_ = 0;
    discard_ = false;
    if (rejected) { return Result::Rejected; }
    return nonempty ? Result::Line : Result::None;
  }
  if (discard_) { return Result::None; }
  if (byte < 32 || byte > 126) {
    discard_ = true;
    rejection_ = "invalid_byte";
  } else if (length_ >= config::lineCapacity - 1) {
    discard_ = true;
    rejection_ = "line_too_long";
  } else {
    buffer_[length_++] = static_cast<char>(byte);
  }
  return Result::None;
}
