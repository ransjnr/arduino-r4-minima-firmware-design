#include <Arduino.h>
#include <string.h>
#include "Config.h"
#include "Controller.h"
#include "Duration.h"

Controller::Controller(int ledPin) : led_(ledPin) {}

void Controller::begin() {
  led_.begin();
  Serial.begin(config::serialBaud);
}

const char* Controller::modeName() const {
  switch (mode_) {
    case Mode::Once: return "once";
    case Mode::Heartbeat: return "heartbeat";
    case Mode::Burst: return "burst";
    case Mode::Alert: return "alert";
    default: return "manual";
  }
}

void Controller::hold(bool on) {
  led_.hold(on);
  mode_ = Mode::Manual;
}

void Controller::start(Mode mode, uint32_t nowMs) {
  mode_ = mode;
  StatusLed::Pattern pattern = StatusLed::Pattern::Heartbeat;
  if (mode == Mode::Burst) { pattern = StatusLed::Pattern::Burst; }
  if (mode == Mode::Alert) { pattern = StatusLed::Pattern::Alert; }
  led_.start(pattern, mode != Mode::Once, nowMs);
}

void Controller::error(const char* reason) {
  rejectedLines_++;
  lastError_ = reason;
  if (strcmp(reason, "line_too_long") == 0 || strcmp(reason, "invalid_byte") == 0) {
    lineErrors_++;
  } else if (strcmp(reason, "unknown_command") == 0) {
    commandErrors_++;
  } else {
    settingErrors_++;
  }
  Serial.print("event=error reason="); Serial.println(reason);
}

void Controller::help() {
  Serial.println("commands=help,version,status,errors,clear_errors,on,off,toggle,blink,stop,mode,set");
  Serial.println("usage=mode manual|heartbeat|burst|alert; set on N; set off N; clear errors");
  Serial.println("range_ms=50..5000 aliases=h:help,b:heartbeat,s:stop,1:on,0:off,?:status");
}

void Controller::status(uint32_t nowMs) {
  // Single-threaded application state is stable while this record is emitted.
  Serial.print("event=status t_ms="); Serial.print(nowMs);
  Serial.print(" mode="); Serial.print(modeName());
  Serial.print(" led="); Serial.print(led_.isOn() ? "ON" : "OFF");
  Serial.print(" on_ms="); Serial.print(led_.onMs());
  Serial.print(" off_ms="); Serial.print(led_.offMs());
  Serial.print(" completed_blinks="); Serial.print(completedBlinks_);
  Serial.print(" completed_patterns="); Serial.print(completedPatterns_);
  Serial.print(" rejected_lines="); Serial.println(rejectedLines_);
}

void Controller::setting(const char* value, bool onSetting, uint32_t nowMs) {
  uint32_t candidate = 0;
  const char* reason = duration::parse(value, candidate);
  if (reason != nullptr) { error(reason); return; }
  const uint32_t onMs = onSetting ? candidate : led_.onMs();
  const uint32_t offMs = onSetting ? led_.offMs() : candidate;
  if (!led_.configure(onMs, offMs)) { error("out_of_range"); return; }
  // Explicit policy: a successful timing edit restarts configured patterns ON.
  if (mode_ == Mode::Once || mode_ == Mode::Heartbeat) { start(mode_, nowMs); }
  Serial.print("event=setting_applied name="); Serial.print(onSetting ? "on" : "off");
  Serial.print(" value_ms="); Serial.println(candidate);
}

void Controller::command(const char* line, uint32_t nowMs) {
  if (strcmp(line, "help") == 0 || strcmp(line, "h") == 0) {
    help();
  } else if (strcmp(line, "version") == 0) {
    Serial.print("event=version firmware="); Serial.println(config::firmware);
  } else if (strcmp(line, "status") == 0 || strcmp(line, "?") == 0) {
    status(nowMs);
  } else if (strcmp(line, "errors") == 0) {
    Serial.print("event=errors total="); Serial.print(rejectedLines_);
    Serial.print(" line="); Serial.print(lineErrors_);
    Serial.print(" command="); Serial.print(commandErrors_);
    Serial.print(" setting="); Serial.print(settingErrors_);
    Serial.print(" last="); Serial.println(lastError_);
  } else if (strcmp(line, "clear errors") == 0) {
    rejectedLines_ = lineErrors_ = commandErrors_ = settingErrors_ = 0;
    lastError_ = "none";
    Serial.println("event=errors_cleared");
  } else if (strcmp(line, "on") == 0 || strcmp(line, "1") == 0) {
    hold(true); Serial.println("event=on led=ON");
  } else if (strcmp(line, "off") == 0 || strcmp(line, "0") == 0 ||
             strcmp(line, "stop") == 0 || strcmp(line, "s") == 0 ||
             strcmp(line, "mode manual") == 0) {
    hold(false); Serial.println("event=stopped mode=manual led=OFF");
  } else if (strcmp(line, "toggle") == 0) {
    hold(!led_.isOn());
    Serial.print("event=toggled led="); Serial.println(led_.isOn() ? "ON" : "OFF");
  } else if (strcmp(line, "blink") == 0) {
    start(Mode::Once, nowMs); Serial.println("event=blink_started");
  } else if (strcmp(line, "mode heartbeat") == 0 || strcmp(line, "b") == 0) {
    start(Mode::Heartbeat, nowMs); Serial.println("event=mode_changed mode=heartbeat");
  } else if (strcmp(line, "mode burst") == 0) {
    start(Mode::Burst, nowMs); Serial.println("event=mode_changed mode=burst");
  } else if (strcmp(line, "mode alert") == 0) {
    start(Mode::Alert, nowMs); Serial.println("event=mode_changed mode=alert");
  } else if (strncmp(line, "set on ", 7) == 0) {
    setting(line + 7, true, nowMs);
  } else if (strncmp(line, "set off ", 8) == 0) {
    setting(line + 8, false, nowMs);
  } else {
    error("unknown_command");
  }
}

void Controller::update() {
  // Bound receive work per pass; partial/rejected lines never block the driver.
  for (unsigned int i = 0; i < config::inputBudget && Serial.available() > 0; i++) {
    const LineReceiver::Result result = receiver_.consume(Serial.read());
    if (result == LineReceiver::Result::Line) {
      command(receiver_.line(), static_cast<uint32_t>(millis()));
    } else if (result == LineReceiver::Result::Rejected) {
      error(receiver_.reason());
    }
  }
  if (led_.update(static_cast<uint32_t>(millis()))) {
    if (mode_ == Mode::Once) {
      completedBlinks_++;
      mode_ = Mode::Manual;
      if (Serial) { Serial.println("event=blink_finished led=OFF"); }
    } else {
      completedPatterns_++;
    }
  }
}
