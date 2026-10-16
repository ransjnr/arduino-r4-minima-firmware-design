// Lesson 02, Exercise 02: filter structured diagnostic records.
const unsigned int logOff = 0;
const unsigned int logInfo = 1;
const unsigned int logDebug = 2;
const unsigned int configuredLogLevel = logDebug;

const unsigned long onTimeMs = 200;
const unsigned long offTimeMs = 800;
const unsigned long summaryEveryCycles = 5;

unsigned long completedCycles = 0;
bool diagnosticsStarted = false;

bool shouldLog(unsigned int level) {
  return Serial && configuredLogLevel >= level;
}

void startDiagnosticsIfReady() {
  if (!diagnosticsStarted && shouldLog(logInfo)) {
    Serial.println("level=INFO event=diagnostics_started lesson=02 exercise=02");
    Serial.print("level=INFO event=config on_ms=");
    Serial.print(onTimeMs);
    Serial.print(" off_ms=");
    Serial.print(offTimeMs);
    Serial.print(" summary_every_cycles=");
    Serial.print(summaryEveryCycles);
    Serial.print(" log_level=");
    Serial.println(configuredLogLevel);
    diagnosticsStarted = true;
  }
}

void logLedState(const char* state) {
  if (!shouldLog(logDebug)) {
    return;
  }
  Serial.print("level=DEBUG event=led_changed completed_cycles=");
  Serial.print(completedCycles);
  Serial.print(" led=");
  Serial.println(state);
}

void logSummary() {
  if (!shouldLog(logInfo)) {
    return;
  }
  Serial.print("level=INFO event=progress completed_cycles=");
  Serial.println(completedCycles);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
}

void loop() {
  // Deferred until a connection is ready, rather than waiting in setup.
  startDiagnosticsIfReady();

  digitalWrite(LED_BUILTIN, HIGH);
  logLedState("ON");
  delay(onTimeMs);

  digitalWrite(LED_BUILTIN, LOW);
  logLedState("OFF");
  delay(offTimeMs);

  completedCycles++;
  // Test the divisor before using remainder: modulo zero is invalid.
  if (summaryEveryCycles > 0 &&
      completedCycles % summaryEveryCycles == 0) {
    logSummary();
  }
}
