#pragma once
#include <stdint.h>
#include "LineReceiver.h"
#include "StatusLed.h"

class Controller {
 public:
  explicit Controller(int ledPin);
  void begin();
  void update();

 private:
  enum class Mode { Manual, Once, Heartbeat, Burst, Alert };
  void command(const char* line, uint32_t nowMs);
  void start(Mode mode, uint32_t nowMs);
  void hold(bool on);
  void status(uint32_t nowMs);
  void error(const char* reason);
  void setting(const char* value, bool onSetting, uint32_t nowMs);
  void help();
  const char* modeName() const;
  StatusLed led_;
  LineReceiver receiver_;
  Mode mode_ = Mode::Manual;
  uint32_t completedBlinks_ = 0;
  uint32_t completedPatterns_ = 0;
  uint32_t rejectedLines_ = 0;
  uint32_t lineErrors_ = 0;
  uint32_t commandErrors_ = 0;
  uint32_t settingErrors_ = 0;
  const char* lastError_ = "none";
};
