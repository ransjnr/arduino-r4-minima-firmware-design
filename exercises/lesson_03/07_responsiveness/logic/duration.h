#pragma once

// Pure contract: text must point to a null-terminated string.
// Success returns nullptr and writes result (50..5000 inclusive).
// Failure returns a fixed reason label and leaves result unchanged.
namespace duration {
constexpr unsigned long minimumMs = 50;
constexpr unsigned long maximumMs = 5000;

inline const char* parse(const char* text, unsigned long& result) {
  if (text[0] == '\0') { return "invalid_number"; }
  for (unsigned int i = 0; text[i] != '\0'; i++) {
    if (text[i] < '0' || text[i] > '9') { return "invalid_number"; }
  }
  unsigned long candidate = 0;
  for (unsigned int i = 0; text[i] != '\0'; i++) {
    const unsigned long digit = static_cast<unsigned long>(text[i] - '0');
    if (candidate > maximumMs / 10 ||
        (candidate == maximumMs / 10 && digit > maximumMs % 10)) {
      return "out_of_range";
    }
    candidate = candidate * 10 + digit;
  }
  if (candidate < minimumMs) { return "out_of_range"; }
  result = candidate;
  return nullptr;
}
} // namespace duration
