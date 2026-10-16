#pragma once
#include <stdint.h>

// Owns the output pin and the timing state for one status indicator.
class StatusLed {
 public:
  explicit StatusLed(int pin);
  void begin();
  void start(uint32_t nowMs);
  void hold(bool on);
  void update(uint32_t nowMs);
  bool isOn() const;
  bool isRunning() const;

 private:
  void write(bool on);
  int pin_;
  bool on_ = false;
  bool running_ = false;
  uint32_t changedAtMs_ = 0;
  static constexpr uint32_t onDurationMs_ = 200;
  static constexpr uint32_t offDurationMs_ = 800;
};
