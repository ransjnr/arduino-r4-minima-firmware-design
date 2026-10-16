// Exercise 05: choose one pattern using conditions.
const unsigned int heartbeatMode = 1;
const unsigned int burstMode = 2;
const unsigned int alertMode = 3;

// Change this value and upload again to select a pattern.
unsigned int selectedMode = heartbeatMode;

void pulseLed(unsigned long onMs, unsigned long offMs) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(onMs);
  digitalWrite(LED_BUILTIN, LOW);
  delay(offMs);
}

void sendBurst(unsigned int count, unsigned long onMs,
               unsigned long offMs) {
  for (unsigned int pulseIndex = 0; pulseIndex < count; pulseIndex++) {
    pulseLed(onMs, offMs);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (selectedMode == heartbeatMode) {
    pulseLed(100, 1900);
  } else if (selectedMode == burstMode) {
    sendBurst(3, 150, 150);
    delay(1100);
  } else if (selectedMode == alertMode) {
    sendBurst(5, 100, 100);
    delay(1000);
  } else {
    // Explicit fallback for an unrecognized mode.
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
