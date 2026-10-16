#include <cstdlib>
#include <iostream>
#include <string>
#include DURATION_HEADER

void require(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    std::exit(1);
  }
}

void rejected(const std::string& text, const char* expected) {
  unsigned long result = 777;
  const char* error = duration::parse(text.c_str(), result);
  require(error && std::string(error) == expected, "reason for [" + text + "]");
  require(result == 777, "failure preserves output for [" + text + "]");
}

int main() {
  // Independent oracle: the protocol accepts integers in the closed interval.
  for (unsigned long n = 0; n <= 10000; n++) {
    unsigned long result = 777;
    const char* error = duration::parse(std::to_string(n).c_str(), result);
    if (n >= 50 && n <= 5000) {
      require(!error && result == n, "accepted value " + std::to_string(n));
    } else {
      require(error && std::string(error) == "out_of_range" && result == 777,
              "rejected value " + std::to_string(n));
    }
  }
  for (const char* text : {"", "-50", "+50", "1.5", "100ms", " 100", "100 ", "1 00", "\t50"}) {
    rejected(text, "invalid_number");
  }
  rejected(std::string(1000, '9'), "out_of_range");
  rejected(std::string(1000, '9') + "x", "invalid_number");
  unsigned long result = 0;
  require(!duration::parse("00050", result) && result == 50, "leading zeros");
  require(!duration::parse((std::string(1000, '0') + "50").c_str(), result) && result == 50,
          "long leading-zero token in pure API");
  std::cout << "PASS parser contract (not USB input-buffer or board timing tests)\n";
}
