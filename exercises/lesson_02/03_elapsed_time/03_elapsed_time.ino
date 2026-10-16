// Lesson 02, Exercise 03: measure intervals, not host display times.
const unsigned int logOff = 0;
const unsigned int logInfo = 1;
const unsigned int logDebug = 2;
const unsigned int configuredLogLevel = logDebug;
const unsigned long onTimeMs = 200;
const unsigned long offTimeMs = 800;

unsigned long completedCycles = 0;
unsigned long previousOnMs = 0;
bool havePreviousOn = false;
bool diagnosticsStarted = false;

bool shouldLog(unsigned int level) {
  return Serial && configuredLogLevel >= level;
}

void logEdge(const char* state, unsigned long eventMs) {
  if (!shouldLog(logDebug)) {
    return;
  }
  Serial.print("level=DEBUG event=led_changed t_ms=");
  Serial.print(eventMs);
  Serial.print(" completed_cycles=");
  Serial.print(completedCycles);
  Serial.print(" led=");
  Serial.println(state);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
}

void loop() {
  if (!diagnosticsStarted && shouldLog(logInfo)) {
    Serial.print("level=INFO event=config on_ms=");
    Serial.print(onTimeMs);
    Serial.print(" off_ms=");
    Serial.print(offTimeMs);
    Serial.print(" log_level=");
    Serial.println(configuredLogLevel);
    diagnosticsStarted = true;
  }

  digitalWrite(LED_BUILTIN, HIGH);
  const unsigned long onAtMs = millis();
  const bool periodAvailable = havePreviousOn;
  const unsigned long periodMs = onAtMs - previousOnMs;
  previousOnMs = onAtMs;
  havePreviousOn = true;
  logEdge("ON", onAtMs);
  delay(onTimeMs);

  digitalWrite(LED_BUILTIN, LOW);
  const unsigned long offAtMs = millis();
  const unsigned long observedOnMs = offAtMs - onAtMs;
  logEdge("OFF", offAtMs);
  delay(offTimeMs);
  const unsigned long cycleFinishedAtMs = millis();
  completedCycles++;

  if (shouldLog(logInfo)) {
    Serial.print("level=INFO event=cycle_finished t_ms=");
    Serial.print(cycleFinishedAtMs);
    Serial.print(" completed_cycles=");
    Serial.print(completedCycles);
    Serial.print(" observed_on_ms=");
    Serial.print(observedOnMs);
    Serial.print(" previous_on_to_this_on_ms=");
    if (periodAvailable) {
      Serial.println(periodMs);
    } else {
      Serial.println("NA");
    }
  }
}
