#include <Arduino.h>
#include "Config.h"
#include "StatusLed.h"

StatusLed::StatusLed(int pin) : pin_(pin) {}

void StatusLed::begin() {
  pinMode(pin_, OUTPUT);
  configure(config::defaultOnMs, config::defaultOffMs);
  hold(false);
}

bool StatusLed::configure(uint32_t onMs, uint32_t offMs) {
  if (onMs < config::minDurationMs || onMs > config::maxDurationMs ||
      offMs < config::minDurationMs || offMs > config::maxDurationMs) {
    return false;
  }
  onMs_ = onMs;
  offMs_ = offMs;
  return true;
}

void StatusLed::write(bool on) {
  digitalWrite(pin_, on ? HIGH : LOW);
  on_ = on;
}

void StatusLed::start(Pattern pattern, bool repeat, uint32_t nowMs) {
  pattern_ = pattern;
  repeat_ = repeat;
  segment_ = 0;
  changedAtMs_ = nowMs;
  running_ = true;
  write(true);
}

void StatusLed::hold(bool on) {
  running_ = false;
  segment_ = 0;
  write(on);
}

uint8_t StatusLed::segmentCount() const {
  if (pattern_ == Pattern::Burst) { return 6; }
  if (pattern_ == Pattern::Alert) { return 10; }
  return 2;
}

uint32_t StatusLed::segmentDuration() const {
  if (pattern_ == Pattern::Heartbeat) {
    return segment_ == 0 ? onMs_ : offMs_;
  }
  // Fixed burst: 3 x (100/100) + 400 off; alert: 5 x (100/100) + 1000 off.
  if (segment_ == segmentCount() - 1) {
    return pattern_ == Pattern::Burst ? 500 : 1100;
  }
  return 100;
}

bool StatusLed::update(uint32_t nowMs) {
  if (!running_ || static_cast<uint32_t>(nowMs - changedAtMs_) < segmentDuration()) {
    return false;
  }
  // A late call produces one real transition, not a catch-up flash burst.
  changedAtMs_ = nowMs;
  segment_++;
  if (segment_ >= segmentCount()) {
    if (repeat_) {
      segment_ = 0;
      write(true);
    } else {
      hold(false);
    }
    return true;
  }
  write(segment_ % 2 == 0);
  return false;
}
