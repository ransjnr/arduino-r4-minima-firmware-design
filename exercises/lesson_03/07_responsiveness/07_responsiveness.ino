// Lesson 03, Exercise 07: see README.md for requirements and tests.
#include <string.h>
#include "logic/duration.h"

const char* firmwareRevision = "lesson03_ex07_v1";
const unsigned int lineCapacity = 32; // 31 content bytes + null terminator.
char lineBuffer[lineCapacity];
unsigned int lineLength = 0;
bool discardingLine = false;
bool previousWasCR = false;
bool ledIsOn = false;
const unsigned long minDurationMs = duration::minimumMs;
const unsigned long maxDurationMs = duration::maximumMs;
unsigned long onTimeMs = 200;
unsigned long offTimeMs = 800;
unsigned long completedBlinks = 0;
unsigned long rejectedCommands = 0;
unsigned long unknownErrors = 0;
unsigned long numericErrors = 0;
unsigned long rangeErrors = 0;
unsigned long lineErrors = 0;
const char* lastError = "none";

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
  const bool snapshotLed = ledIsOn;
  const unsigned long snapshotOnMs = onTimeMs;
  const unsigned long snapshotOffMs = offTimeMs;
  const unsigned long snapshotBlinks = completedBlinks;
  const unsigned long snapshotErrors = rejectedCommands;

  Serial.print("event=status captured_at_ms=");
  Serial.print(snapshotMs);
  Serial.print(" mode=manual led=");
  Serial.print(snapshotLed ? "ON" : "OFF");
  Serial.print(" on_ms=");
  Serial.print(snapshotOnMs);
  Serial.print(" off_ms=");
  Serial.print(snapshotOffMs);
  Serial.print(" completed_blinks=");
  Serial.print(snapshotBlinks);
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
  return duration::parse(text, result);
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

// Test-fixture command; not a board reset or a persistence test.
void resetTestState() {
  setLed(false);
  onTimeMs = 200;
  offTimeMs = 800;
  completedBlinks = 0;
  rejectedCommands = 0;
  unknownErrors = numericErrors = rangeErrors = lineErrors = 0;
  lastError = "none";
  Serial.println("event=test_reset");
}

void handleLine(const char* line) {
  if (strcmp(line, "help") == 0) {
    Serial.println("event=help commands=help,version,status,errors,on,off,blink,test_reset");
    Serial.println("usage=set on N | set off N | clear errors | test reset; N=50..5000 milliseconds");
  } else if (strcmp(line, "test reset") == 0) {
    resetTestState();
  } else if (strcmp(line, "version") == 0) {
    Serial.print("event=version firmware=");
    Serial.println(firmwareRevision);
  } else if (strcmp(line, "status") == 0) {
    printStatus();
  } else if (strcmp(line, "errors") == 0) {
    printErrors();
  } else if (strcmp(line, "clear errors") == 0) {
    clearErrors();
  } else if (strcmp(line, "on") == 0) {
    setLed(true);
    Serial.println("event=on led=ON");
  } else if (strcmp(line, "off") == 0) {
    setLed(false);
    Serial.println("event=off led=OFF");
  } else if (strcmp(line, "blink") == 0) {
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

void loop() {
  if (Serial.available() > 0) {
    consumeByte(Serial.read());
  }
}
