// Exercise 08: initialization once, operational behavior repeatedly.
const unsigned int startupPulseCount = 3;
const unsigned long startupOnMs = 100;
const unsigned long startupOffMs = 100;
const unsigned long startupPauseMs = 1400;
const unsigned long heartbeatOnMs = 100;
const unsigned long heartbeatOffMs = 1900;

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
  sendBurst(startupPulseCount, startupOnMs, startupOffMs);
  delay(startupPauseMs);
}

void loop() {
  pulseLed(heartbeatOnMs, heartbeatOffMs);
}
