// Lesson 02, Exercise 05: bounded, newline-terminated commands.
#include <string.h>

const unsigned int lineCapacity = 16; // 15 content bytes + null terminator.
char lineBuffer[lineCapacity];
unsigned int lineLength = 0;
bool discardingLine = false;
bool previousWasCR = false;
bool ledIsOn = false;

void setLed(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  ledIsOn = on;
}

void handleLine(const char* line) {
  if (strcmp(line, "help") == 0) {
    Serial.println("event=help commands=help,on,off,blink");
  } else if (strcmp(line, "on") == 0) {
    setLed(true);
    Serial.println("event=on led=ON");
  } else if (strcmp(line, "off") == 0) {
    setLed(false);
    Serial.println("event=off led=OFF");
  } else if (strcmp(line, "blink") == 0) {
    setLed(true);
    Serial.println("event=blink_started");
    delay(200);
    setLed(false);
    delay(800);
    Serial.println("event=blink_finished led=OFF");
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
