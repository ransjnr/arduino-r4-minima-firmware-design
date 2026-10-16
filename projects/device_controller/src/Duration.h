#pragma once
#include <stdint.h>
#include "Config.h"

namespace duration {
// Input must be a non-null, null-terminated string. Failure preserves output.
inline const char* parse(const char* text, uint32_t& output) {
  if (*text == '\0') { return "invalid_number"; }
  for (unsigned int i = 0; text[i] != '\0'; i++) {
    if (text[i] < '0' || text[i] > '9') { return "invalid_number"; }
  }
  uint32_t value = 0;
  for (unsigned int i = 0; text[i] != '\0'; i++) {
    const uint32_t digit = static_cast<uint32_t>(text[i] - '0');
    if (value > config::maxDurationMs / 10 ||
        (value == config::maxDurationMs / 10 && digit > config::maxDurationMs % 10)) {
      return "out_of_range";
    }
    value = value * 10 + digit;
  }
  if (value < config::minDurationMs) { return "out_of_range"; }
  output = value;
  return nullptr;
}
}
