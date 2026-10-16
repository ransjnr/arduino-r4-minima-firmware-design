// Lesson 02, Exercise 04: one byte is one command.
const unsigned long blinkOnMs = 200;
const unsigned long blinkOffMs = 800;
bool ledIsOn = false;

void setLed(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  ledIsOn = on;
}

void printReply(const char* event) {
  if (!Serial) {
    return;
  }
  const unsigned long handledAtMs = millis();
  Serial.print("event=");
  Serial.print(event);
  Serial.print(" handled_at_ms=");
  Serial.print(handledAtMs);
  Serial.print(" led=");
  Serial.println(ledIsOn ? "ON" : "OFF");
}

void printHelp() {
  if (Serial) {
    Serial.println("event=help commands=h:help,1:on,0:off,b:blink_once");
  }
}

void handleCommand(int command) {
  if (command == '\r' || command == '\n') {
    return;
  }

  if (command == 'h') {
    printHelp();
  } else if (command == '1') {
    setLed(true);
    printReply("on");
  } else if (command == '0') {
    setLed(false);
    printReply("off");
  } else if (command == 'b') {
    setLed(true);
    printReply("blink_started");
    delay(blinkOnMs);
    setLed(false);
    delay(blinkOffMs);
    printReply("blink_finished");
  } else {
    if (Serial) {
      Serial.print("event=error reason=unknown_command byte_decimal=");
      Serial.println(command);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  setLed(false);
  Serial.begin(115200);
}

void loop() {
  // Process at most one byte per loop call; never wait for input here.
  if (Serial.available() > 0) {
    const int command = Serial.read();
    if (command >= 0) {
      handleCommand(command);
    }
  }
}
