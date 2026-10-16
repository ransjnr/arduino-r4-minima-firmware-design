// Exercise 09: select exactly one deliberate fault, or 0 for baseline.
// 1 = wrong units; 2 = misplaced pause; 3 = off-by-one repetition.
const unsigned int faultCase = 0;
const unsigned int expectedCount = 3;
const unsigned long onTimeMs = 200;
const unsigned long offTimeMs = 200;
const unsigned long burstPauseMs = 800;

void pulseLed(unsigned long onMs, unsigned long offMs) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(onMs);
  digitalWrite(LED_BUILTIN, LOW);
  delay(offMs);
}

void sendBurst(unsigned int count, unsigned long onMs,
               unsigned long offMs) {
  for (unsigned int i = 0; i < count; i++) {
    pulseLed(onMs, offMs);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (faultCase == 0) {
    sendBurst(expectedCount, onTimeMs, offTimeMs);
    delay(burstPauseMs);
  } else if (faultCase == 1) {
    // Deliberate defect: durations are expressed in milliseconds.
    sendBurst(expectedCount, 2, offTimeMs);
    delay(burstPauseMs);
  } else if (faultCase == 2) {
    for (unsigned int i = 0; i < expectedCount; i++) {
      pulseLed(onTimeMs, offTimeMs);
      // Deliberate defect: group pause is inside the repetition.
      delay(burstPauseMs);
    }
  } else if (faultCase == 3) {
    // Deliberate defect: includes the upper bound as an extra iteration.
    for (unsigned int i = 0; i <= expectedCount; i++) {
      pulseLed(onTimeMs, offTimeMs);
    }
    delay(burstPauseMs);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
