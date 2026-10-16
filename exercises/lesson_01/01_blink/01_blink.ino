// Exercise 01: control the UNO R4 Minima's built-in LED.

void setup() {
  // Configure the LED pin to drive an output signal.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
