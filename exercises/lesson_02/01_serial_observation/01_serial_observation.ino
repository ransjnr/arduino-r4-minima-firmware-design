// Lesson 02, Exercise 01: observe LED state and progress through USB Serial.
const unsigned long onTimeMs = 200;
const unsigned long offTimeMs = 800;
unsigned long completedCycles = 0;

void printState(const char* ledState) {
  Serial.print("completed_cycles=");
  Serial.print(completedCycles);
  Serial.print(" led=");
  Serial.println(ledState);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  // Deliberately no infinite wait for Serial Monitor.
  // Startup output may be missed if the monitor is not open yet.
  if (Serial) {
    Serial.println("BOOT lesson=02 exercise=01");
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  if (Serial) {
    printState("ON");
  }
  delay(onTimeMs);

  digitalWrite(LED_BUILTIN, LOW);
  if (Serial) {
    printState("OFF");
  }
  delay(offTimeMs);

  // A cycle is complete only after both intervals finish.
  completedCycles++;
}
