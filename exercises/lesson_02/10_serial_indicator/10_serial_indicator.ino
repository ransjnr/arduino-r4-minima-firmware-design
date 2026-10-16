// Lesson 02, Exercise 10: serial-controlled status indicator capstone.
#include <string.h>

const unsigned int lineCapacity = 32; // 31 content bytes + null terminator.
char lineBuffer[lineCapacity];
unsigned int lineLength = 0;
bool discardingLine = false;
bool previousWasCR = false;
bool ledIsOn = false;
const unsigned long minDurationMs = 50;
const unsigned long maxDurationMs = 5000;
unsigned long onTimeMs = 200;
unsigned long offTimeMs = 800;
unsigned long completedBlinks = 0;
unsigned long rejectedCommands = 0;
unsigned long unknownErrors = 0;
unsigned long numericErrors = 0;
unsigned long rangeErrors = 0;
unsigned long lineErrors = 0;
const char* lastError = "none";
const unsigned int manualMode = 0;
const unsigned int heartbeatMode = 1;
const unsigned int burstMode = 2;
unsigned int selectedMode = manualMode;
unsigned long completedPatterns = 0;

const char* modeName() {
  if (selectedMode == heartbeatMode) { return "heartbeat"; }
  if (selectedMode == burstMode) { return "burst"; }
  return "manual";
}

void reportError(const char* reason) {
  rejectedCommands++;
  if (strcmp(reason, "unknown_command") == 0) {
    unknownErrors++;
  } else if (strcmp(reason, "invalid_number") == 0) {
    numericErrors++;
  } else if (strcmp(reason, "out_of_range") == 0) {
    rangeErrors++;
  } else {
    lineErrors++;
  }
  lastError = reason; // All callers supply fixed string literals.
  Serial.print("event=error reason=");
  Serial.println(reason);
}

void printErrors() {
  Serial.print("event=errors total="); Serial.print(rejectedCommands);
  Serial.print(" unknown="); Serial.print(unknownErrors);
  Serial.print(" numeric="); Serial.print(numericErrors);
  Serial.print(" range="); Serial.print(rangeErrors);
  Serial.print(" line="); Serial.print(lineErrors);
  Serial.print(" last="); Serial.println(lastError);
}

void clearErrors() {
  rejectedCommands = 0;
  unknownErrors = 0;
  numericErrors = 0;
  rangeErrors = 0;
  lineErrors = 0;
  lastError = "none";
  Serial.println("event=errors_cleared");
}

void printStatus() {
  // Capture the reported values before outputting the record.
  const unsigned long snapshotMs = millis();
  const char* snapshotMode = modeName();
  const unsigned long snapshotPatterns = completedPatterns;
  const bool snapshotLed = ledIsOn;
  const unsigned long snapshotOnMs = onTimeMs;
  const unsigned long snapshotOffMs = offTimeMs;
  const unsigned long snapshotBlinks = completedBlinks;
  const unsigned long snapshotErrors = rejectedCommands;

  Serial.print("event=status captured_at_ms=");
  Serial.print(snapshotMs);
  Serial.print(" mode=");
  Serial.print(snapshotMode);
  Serial.print(" led=");
  Serial.print(snapshotLed ? "ON" : "OFF");
  Serial.print(" on_ms=");
  Serial.print(snapshotOnMs);
  Serial.print(" off_ms=");
  Serial.print(snapshotOffMs);
  Serial.print(" completed_blinks=");
  Serial.print(snapshotBlinks);
  Serial.print(" completed_patterns=");
  Serial.print(snapshotPatterns);
  Serial.print(" rejected_commands=");
  Serial.println(snapshotErrors);
}

void setLed(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  ledIsOn = on;
}

// Return a fixed error label, or nullptr on success.
// Only update the caller's result after every validation succeeds.
const char* parseDuration(const char* text, unsigned long& result) {
  if (text[0] == '\0') {
    return "invalid_number";
  }
  // Check the entire token first: no signs, spaces, units, or suffixes.
  for (unsigned int i = 0; text[i] != '\0'; i++) {
    if (text[i] < '0' || text[i] > '9') {
      return "invalid_number";
    }
  }

  unsigned long candidate = 0;
  for (unsigned int i = 0; text[i] != '\0'; i++) {
    const unsigned long digit = static_cast<unsigned long>(text[i] - '0');
    // Check BEFORE multiply/add. The bound is far below type overflow.
    if (candidate > maxDurationMs / 10 ||
        (candidate == maxDurationMs / 10 && digit > maxDurationMs % 10)) {
      return "out_of_range";
    }
    candidate = candidate * 10 + digit;
  }
  if (candidate < minDurationMs) {
    return "out_of_range";
  }
  result = candidate;
  return nullptr;
}

void applySetting(const char* name, const char* text, unsigned long& target) {
  unsigned long candidate = 0;
  const char* error = parseDuration(text, candidate);
  if (error != nullptr) {
    reportError(error);
    return;
  }
  target = candidate; // Commit only the fully validated value.
  Serial.print("event=setting_applied name=");
  Serial.print(name);
  Serial.print(" value_ms=");
  Serial.println(target);
}

void handleLine(const char* line) {
  if (strcmp(line, "help") == 0) {
    Serial.println("event=help commands=help,status,errors,clear_errors,on,off,blink,set_on,set_off,mode,stop");
    Serial.println("usage=set on N | set off N | clear errors | mode manual/heartbeat/burst | stop; N=50..5000 ms");
  } else if (strcmp(line, "status") == 0) {
    printStatus();
  } else if (strcmp(line, "errors") == 0) {
    printErrors();
  } else if (strcmp(line, "clear errors") == 0) {
    clearErrors();
  } else if (strcmp(line, "on") == 0) {
    selectedMode = manualMode;
    setLed(true);
    Serial.println("event=on led=ON");
  } else if (strcmp(line, "off") == 0) {
    selectedMode = manualMode;
    setLed(false);
    Serial.println("event=off led=OFF");
  } else if (strcmp(line, "blink") == 0) {
    selectedMode = manualMode;
    setLed(true);
    Serial.print("event=blink_started on_ms=");
    Serial.print(onTimeMs);
    Serial.print(" off_ms=");
    Serial.println(offTimeMs);
    delay(onTimeMs);
    setLed(false);
    delay(offTimeMs);
    completedBlinks++;
    Serial.println("event=blink_finished led=OFF");
  } else if (strcmp(line, "mode manual") == 0 || strcmp(line, "stop") == 0) {
    selectedMode = manualMode;
    setLed(false);
    Serial.println("event=mode_changed mode=manual led=OFF");
  } else if (strcmp(line, "mode heartbeat") == 0) {
    selectedMode = heartbeatMode;
    setLed(false);
    Serial.println("event=mode_changed mode=heartbeat");
  } else if (strcmp(line, "mode burst") == 0) {
    selectedMode = burstMode;
    setLed(false);
    Serial.println("event=mode_changed mode=burst");
  } else if (strncmp(line, "set on ", 7) == 0) {
    applySetting("on", line + 7, onTimeMs);
  } else if (strncmp(line, "set off ", 8) == 0) {
    applySetting("off", line + 8, offTimeMs);
  } else {
    reportError("unknown_command");
  }
}

void finishLine() {
  if (discardingLine) {
    reportError("line_rejected");
  } else if (lineLength > 0) {
    lineBuffer[lineLength] = '\0';
    handleLine(lineBuffer);
  }
  lineLength = 0;
  discardingLine = false;
}

void consumeByte(int incoming) {
  if (incoming < 0 || incoming > 255) {
    return;
  }
  if (incoming == '\n' && previousWasCR) {
    previousWasCR = false; // CR+LF is one line boundary.
    return;
  }
  previousWasCR = (incoming == '\r');
  if (incoming == '\r' || incoming == '\n') {
    finishLine();
    return;
  }
  if (discardingLine) {
    return; // Never execute the tail of a rejected line.
  }
  if (incoming < 32 || incoming > 126 || lineLength >= lineCapacity - 1) {
    discardingLine = true;
    return;
  }
  lineBuffer[lineLength] = static_cast<char>(incoming);
  lineLength++;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  setLed(false);
  Serial.begin(115200);
}

void runAutomaticPattern() {
  if (selectedMode == heartbeatMode) {
    setLed(true);
    delay(onTimeMs);
    setLed(false);
    delay(offTimeMs);
    completedPatterns++;
  } else if (selectedMode == burstMode) {
    for (unsigned int i = 0; i < 3; i++) {
      setLed(true);
      delay(100);
      setLed(false);
      delay(100);
    }
    delay(400);
    completedPatterns++;
  }
}

void loop() {
  if (Serial.available() > 0) {
    consumeByte(Serial.read());
  }
  // Drain currently queued input before starting another blocking pattern.
  if (Serial.available() == 0) {
    runAutomaticPattern();
  }
}
