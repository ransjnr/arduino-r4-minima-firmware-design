#include "src/StatusLed.h"

StatusLed statusLed(LED_BUILTIN);

void handleCommand(int command) {
  if (command == '\r' || command == '\n') {
    return;
  }
  if (command == 'b') {
    statusLed.start(static_cast<uint32_t>(millis()));
    Serial.println("mode=blinking");
  } else if (command == 's' || command == '0') {
    statusLed.hold(false);
    Serial.println("mode=manual led=OFF");
  } else if (command == '1') {
    statusLed.hold(true);
    Serial.println("mode=manual led=ON");
  } else if (command == '?') {
    Serial.print("mode=");
    Serial.print(statusLed.isRunning() ? "blinking" : "manual");
    Serial.print(" led=");
    Serial.println(statusLed.isOn() ? "ON" : "OFF");
  } else if (command == 'h') {
    Serial.println("b=start s=stop 1=hold_on 0=hold_off ?=status h=help");
  } else {
    Serial.println("error=unknown_command");
  }
}

void setup() {
  statusLed.begin();
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    const int command = Serial.read();
    if (command >= 0) {
      handleCommand(command);
    }
  }
  statusLed.update(static_cast<uint32_t>(millis()));
}
