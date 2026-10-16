// Exercise 03: short, short, long, followed by a quiet gap.
const unsigned long shortPulseMs = 200;
const unsigned long longPulseMs = 600;
const unsigned long pulseGapMs = 200;
const unsigned long signalGapMs = 1800;

// Parameters receive the durations supplied by each caller.
void pulseLed(unsigned long onMs, unsigned long offMs) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(onMs);
  digitalWrite(LED_BUILTIN, LOW);
  delay(offMs);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  pulseLed(shortPulseMs, pulseGapMs);
  pulseLed(shortPulseMs, pulseGapMs);
  pulseLed(longPulseMs, signalGapMs);
}
