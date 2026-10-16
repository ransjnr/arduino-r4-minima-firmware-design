#include <Arduino.h>
#include <cstdlib>
#include <iostream>
#include <limits>
#include "Controller.h"
#include "Duration.h"

uint32_t fakeNowMs = 0;
int fakePinState = LOW;
unsigned int fakePinWrites = 0;
FakeSerial Serial;
void pinMode(int, int) {}
void digitalWrite(int, int state) { fakePinState = state; fakePinWrites++; }
unsigned long millis() { return fakeNowMs; }

void require(bool ok, const char* message) {
  if (!ok) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}
void send(Controller& controller, const std::string& line) {
  for (unsigned char byte : line) { Serial.input.push_back(byte); }
  Serial.input.push_back('\n');
  while (Serial.available()) { controller.update(); }
}
std::string status(Controller& controller) {
  Serial.output.str(""); Serial.output.clear();
  send(controller, "status");
  return Serial.output.str();
}
void contains(const std::string& text, const std::string& expected, const char* message) {
  require(text.find(expected) != std::string::npos, message);
}

int main() {
  uint32_t value = 777;
  for (uint32_t i = 0; i <= 10000; i++) {
    value = 777;
    const char* result = duration::parse(std::to_string(i).c_str(), value);
    if (i >= 50 && i <= 5000) { require(!result && value == i, "numeric interval"); }
    else { require(result && value == 777, "failure preserves output"); }
  }
  for (const char* invalid : {"", "-50", "50ms", " 50", "50 ", "1.5", "9999999999999999999999999"}) {
    value = 777; require(duration::parse(invalid, value) && value == 777, "malformed/huge token");
  }

  LineReceiver receiver;
  require(receiver.consume('o') == LineReceiver::Result::None, "partial command");
  receiver.consume('n');
  require(receiver.consume('\r') == LineReceiver::Result::Line, "CR dispatch");
  require(std::string(receiver.line()) == "on", "complete content");
  require(receiver.consume('\n') == LineReceiver::Result::None, "CRLF single boundary");
  for (int i = 0; i < 63; i++) { receiver.consume('x'); }
  require(receiver.consume('\n') == LineReceiver::Result::Line, "63 bytes fit");
  for (int i = 0; i < 64; i++) { receiver.consume('x'); }
  receiver.consume('o'); receiver.consume('n');
  require(receiver.consume('\n') == LineReceiver::Result::Rejected, "oversized tail discarded");
  receiver.consume(0);
  require(receiver.consume('\n') == LineReceiver::Result::Rejected, "NUL rejected");

  StatusLed driver(LED_BUILTIN); driver.begin();
  const uint32_t nearWrap = std::numeric_limits<uint32_t>::max() - 99;
  driver.start(StatusLed::Pattern::Heartbeat, false, nearWrap);
  require(!driver.update(99) && driver.isOn(), "199 ms across rollover");
  require(!driver.update(100) && !driver.isOn(), "200 ms across rollover");
  require(driver.update(900) && !driver.isRunning() && !driver.isOn(), "once completes off");
  require(!driver.configure(0, 800) && driver.onMs() == 200, "driver validates configuration");
  driver.start(StatusLed::Pattern::Burst, true, 0);
  for (uint32_t t = 100; t <= 500; t += 100) { require(!driver.update(t), "burst intermediate"); }
  require(!driver.isOn() && driver.update(1000) && driver.isOn(), "burst period 1000 ms");
  driver.start(StatusLed::Pattern::Alert, true, 0);
  for (uint32_t t = 100; t <= 900; t += 100) { driver.update(t); }
  require(!driver.isOn() && driver.update(2000), "alert period 2000 ms");
  driver.start(StatusLed::Pattern::Heartbeat, true, 0);
  require(!driver.update(10000) && !driver.isOn(), "late call advances one edge");

  fakeNowMs = 0;
  Controller controller(LED_BUILTIN); controller.begin();
  contains(status(controller), "mode=manual led=OFF on_ms=200 off_ms=800", "startup defaults");
  send(controller, "set on 2000"); send(controller, "set off 3000"); send(controller, "b");
  fakeNowMs = 100; send(controller, "s");
  require(fakePinState == LOW, "stop during long ON without advancing fake clock");
  contains(status(controller), "mode=manual led=OFF", "stop enters manual");
  send(controller, "on"); send(controller, "set on bad");
  contains(status(controller), "led=ON on_ms=2000 off_ms=3000", "rejection preserves hold/settings");
  send(controller, std::string(64, 'x') + "off");
  require(fakePinState == HIGH, "oversized suffix cannot execute off");
  send(controller, "off"); require(fakePinState == LOW, "parser recovers");
  send(controller, "set on 200"); send(controller, "set off 800");
  fakeNowMs = 1000; send(controller, "blink");
  fakeNowMs = 1200; controller.update(); require(fakePinState == LOW, "once off phase");
  fakeNowMs = 2000; controller.update();
  contains(status(controller), "completed_blinks=1", "once counted on full completion");
  send(controller, "blink"); send(controller, "stop"); fakeNowMs = 3000; controller.update();
  contains(status(controller), "completed_blinks=1", "cancelled once not counted");
  send(controller, "mode heartbeat"); fakeNowMs = 3100; controller.update();
  send(controller, "set on 500"); require(fakePinState == HIGH, "timing edit restarts heartbeat");
  fakeNowMs = 3599; controller.update(); require(fakePinState == HIGH, "restarted duration not early");
  fakeNowMs = 3600; controller.update(); require(fakePinState == LOW, "restarted duration reached");
  send(controller, "toggle");
  contains(status(controller), "mode=manual led=ON", "toggle cancels automatic mode");
  send(controller, "clear errors");
  contains(status(controller), "rejected_lines=0", "clear errors");
  send(controller, "stop");
  std::cout << "PASS: parser, buffer recovery, timing, rollover, patterns, stop, settings and command state\n";
}
