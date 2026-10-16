// Exercise 07: shorten pulses within explicit limits, then restart.
const unsigned long minOnMs = 100;
const unsigned long maxOnMs = 500;
const unsigned long stepMs = 100;
const unsigned long offTimeMs = 300;
const unsigned long sequencePauseMs = 1200;

unsigned long currentOnMs = maxOnMs;

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
  if (minOnMs == 0 || minOnMs > maxOnMs || stepMs == 0) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    return;
  }

  // Restore the invariant before generating a pulse.
  if (currentOnMs < minOnMs || currentOnMs > maxOnMs) {
    currentOnMs = maxOnMs;
  }

  pulseLed(currentOnMs, offTimeMs);

  if (currentOnMs == minOnMs) {
    // The minimum pulse has finished; pause and restart.
    delay(sequencePauseMs);
    currentOnMs = maxOnMs;
  } else if (stepMs >= currentOnMs - minOnMs) {
    // Clamp to the minimum rather than stepping past it.
    currentOnMs = minOnMs;
  } else {
    currentOnMs -= stepMs;
  }
}
