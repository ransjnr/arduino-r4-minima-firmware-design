// Exercise 04: repeat an action with a bounded for loop.
const unsigned int pulseCount = 3;
const unsigned long onTimeMs = 200;
const unsigned long pulseGapMs = 200;
// Additional quiet time after the final pulse's normal off interval.
const unsigned long burstPauseMs = 1400;

void pulseLed(unsigned long onMs, unsigned long offMs) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(onMs);
  digitalWrite(LED_BUILTIN, LOW);
  delay(offMs);
}

void sendBurst(unsigned int count) {
  for (unsigned int pulseIndex = 0; pulseIndex < count; pulseIndex++) {
    pulseLed(onTimeMs, pulseGapMs);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  sendBurst(pulseCount);
  delay(burstPauseMs);
}
