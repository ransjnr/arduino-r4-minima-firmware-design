#include <Arduino.h>
#include "StatusLed.h"

StatusLed::StatusLed(int pin) : pin_(pin) {}

void StatusLed::begin() {
  pinMode(pin_, OUTPUT);
  hold(false);
}

void StatusLed::write(bool on) {
  digitalWrite(pin_, on ? HIGH : LOW);
  on_ = on;
}

void StatusLed::start(uint32_t nowMs) {
  running_ = true;
  changedAtMs_ = nowMs;
  write(true);
}

void StatusLed::hold(bool on) {
  running_ = false;
  write(on);
}

void StatusLed::update(uint32_t nowMs) {
  if (!running_) {
    return;
  }
  const uint32_t durationMs = on_ ? onDurationMs_ : offDurationMs_;
  if (static_cast<uint32_t>(nowMs - changedAtMs_) >= durationMs) {
    write(!on_);
    changedAtMs_ = nowMs;
  }
}

bool StatusLed::isOn() const { return on_; }
bool StatusLed::isRunning() const { return running_; }
