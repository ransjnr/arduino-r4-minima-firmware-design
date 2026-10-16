// Exercise 06: preserve state and alternate patterns automatically.
const unsigned int heartbeatMode = 1;
const unsigned int burstMode = 2;
const unsigned int burstsPerMode = 3;

// These variables retain their values between calls to loop().
unsigned int selectedMode = heartbeatMode;
unsigned int completedBursts = 0;

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
  // Recover to a defined state if the configuration is invalid.
  if (burstsPerMode == 0 ||
      (selectedMode != heartbeatMode && selectedMode != burstMode)) {
    digitalWrite(LED_BUILTIN, LOW);
    selectedMode = heartbeatMode;
    completedBursts = 0;
    delay(100);
    return;
  }

  if (selectedMode == heartbeatMode) {
    pulseLed(100, 1900);
  } else {
    sendBurst(3, 150, 150);
    delay(1100);
  }

  // Count one completed pattern, not individual flashes.
  completedBursts++;

  if (completedBursts >= burstsPerMode) {
    completedBursts = 0;
    if (selectedMode == heartbeatMode) {
      selectedMode = burstMode;
    } else {
      selectedMode = heartbeatMode;
    }
  }
}
