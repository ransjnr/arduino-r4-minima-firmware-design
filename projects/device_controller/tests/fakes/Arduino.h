#pragma once
#include <stdint.h>
#include <deque>
#include <sstream>
#include <string>

constexpr int OUTPUT = 1;
constexpr int HIGH = 1;
constexpr int LOW = 0;
constexpr int LED_BUILTIN = 13;
extern uint32_t fakeNowMs;
extern int fakePinState;
extern unsigned int fakePinWrites;
void pinMode(int pin, int mode);
void digitalWrite(int pin, int state);
unsigned long millis();

class FakeSerial {
 public:
  std::deque<int> input;
  std::ostringstream output;
  bool connected = true;
  void begin(unsigned long) {}
  explicit operator bool() const { return connected; }
  int available() const { return static_cast<int>(input.size()); }
  int read() {
    if (input.empty()) { return -1; }
    int result = input.front(); input.pop_front(); return result;
  }
  template<class T> void print(T value) { output << value; }
  template<class T> void println(T value) { output << value << '\n'; }
};
extern FakeSerial Serial;
