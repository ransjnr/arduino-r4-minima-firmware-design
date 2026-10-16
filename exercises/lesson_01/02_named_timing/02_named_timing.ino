// Exercise 02: name timing values instead of repeating raw numbers.
const unsigned long onTimeMs = 500;
const unsigned long offTimeMs = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(onTimeMs);

  digitalWrite(LED_BUILTIN, LOW);
  delay(offTimeMs);
}
