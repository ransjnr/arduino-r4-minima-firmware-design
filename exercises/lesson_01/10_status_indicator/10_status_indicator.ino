// Exercise 10: integrate a startup signal and automatic status sequence.
const unsigned int heartbeatMode = 1;
const unsigned int activityMode = 2;
const unsigned int alertMode = 3;

const unsigned int heartbeatCycles = 2;
const unsigned int activityCycles = 3;
const unsigned int alertCycles = 2;

unsigned int selectedMode = heartbeatMode;
unsigned int completedCycles = 0;

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

bool configurationIsValid() {
  return heartbeatCycles > 0 && activityCycles > 0 && alertCycles > 0;
}

bool modeIsValid(unsigned int mode) {
  return mode == heartbeatMode || mode == activityMode || mode == alertMode;
}

unsigned int cyclesForMode(unsigned int mode) {
  if (mode == heartbeatMode) {
    return heartbeatCycles;
  } else if (mode == activityMode) {
    return activityCycles;
  } else if (mode == alertMode) {
    return alertCycles;
  }
  return 0;
}

void showPattern(unsigned int mode) {
  if (mode == heartbeatMode) {
    pulseLed(100, 1900);
  } else if (mode == activityMode) {
    sendBurst(3, 150, 150);
    delay(1100);
  } else if (mode == alertMode) {
    sendBurst(5, 100, 100);
    delay(1000);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void advanceMode() {
  if (selectedMode == heartbeatMode) {
    selectedMode = activityMode;
  } else if (selectedMode == activityMode) {
    selectedMode = alertMode;
  } else {
    selectedMode = heartbeatMode;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Do not announce startup as successful with invalid configuration.
  if (!configurationIsValid()) {
    return;
  }
  sendBurst(3, 100, 100);
  delay(1400);
}

void loop() {
  if (!configurationIsValid()) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    return;
  }

  if (!modeIsValid(selectedMode)) {
    digitalWrite(LED_BUILTIN, LOW);
    selectedMode = heartbeatMode;
    completedCycles = 0;
    delay(100);
    return;
  }

  showPattern(selectedMode);
  completedCycles++;

  if (completedCycles >= cyclesForMode(selectedMode)) {
    completedCycles = 0;
    advanceMode();
  }
}
