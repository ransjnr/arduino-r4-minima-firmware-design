// Lesson 02, Exercise 06: validate numeric settings before committing.
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
    Serial.print("event=error reason=");
    Serial.println(error);
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
    Serial.println("event=help commands=help,on,off,blink,set_on,set_off");
    Serial.println("usage=set on N | set off N; N=50..5000 milliseconds");
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
    Serial.println("event=blink_finished led=OFF");
  } else if (strncmp(line, "set on ", 7) == 0) {
    applySetting("on", line + 7, onTimeMs);
  } else if (strncmp(line, "set off ", 8) == 0) {
    applySetting("off", line + 8, offTimeMs);
  } else {
    Serial.println("event=error reason=unknown_command");
  }
}

void finishLine() {
  if (discardingLine) {
    Serial.println("event=error reason=line_rejected");
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
