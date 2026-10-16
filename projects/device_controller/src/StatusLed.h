#pragma once
#include <stdint.h>

class StatusLed {
 public:
  enum class Pattern { Heartbeat, Burst, Alert };
  explicit StatusLed(int pin);
  void begin();
  bool configure(uint32_t onMs, uint32_t offMs);
  void start(Pattern pattern, bool repeat, uint32_t nowMs);
  void hold(bool on);
  // Advance at most one edge; return true when a complete cycle finishes.
  bool update(uint32_t nowMs);
  bool isOn() const { return on_; }
  bool isRunning() const { return running_; }
  uint32_t onMs() const { return onMs_; }
  uint32_t offMs() const { return offMs_; }

 private:
  void write(bool on);
  uint8_t segmentCount() const;
  uint32_t segmentDuration() const;
  int pin_;
  bool on_ = false;
  bool running_ = false;
  bool repeat_ = false;
  Pattern pattern_ = Pattern::Heartbeat;
  uint8_t segment_ = 0;
  uint32_t changedAtMs_ = 0;
  uint32_t onMs_ = 200;
  uint32_t offMs_ = 800;
};
