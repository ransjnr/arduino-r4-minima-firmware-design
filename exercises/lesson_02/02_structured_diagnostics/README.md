# Exercise 02 — Structured diagnostics and log levels

## Goal

Turn an informal text trace into predictable diagnostic records. Separate configuration from state transitions and progress summaries, and select the amount of detail without changing LED behavior.

Open `02_structured_diagnostics.ino`, verify/upload for UNO R4 Minima, and open Serial Monitor at 115200. No extra wiring or libraries are needed.

## Record format

Every record occupies one line and uses space-separated `key=value` fields. Fixed labels contain no spaces, so a reader can distinguish fields easily. This is our simple text convention, not JSON or a universal logging standard.

| Record | Level | When and why |
|---|---|---|
| diagnostics_started | INFO | Once per program run, when an INFO-capable connection is first detected |
| config | INFO | Alongside diagnostics_started; reports the actual configuration constants |
| led_changed | DEBUG | After each commanded LED transition |
| progress | INFO | After every five completed cycles by default |

The diagnostics_started record announces the start of reporting, not necessarily the instant of board startup. A monitor can connect after many LED cycles. Deferring this announcement avoids relying on a single print in setup. It is attempted once when the connection appears ready; delivery is still not an acknowledgement from the computer.

## Log-level filtering

| configuredLogLevel | Output |
|---|---|
| logOff (0) | No diagnostic output; LED continues |
| logInfo (1) | Announcement, configuration, and occasional summaries |
| logDebug (2) | All INFO records plus each LED transition |

`shouldLog` returns true only when Serial is ready and the configured level is at least the record's level. This threshold works because we assigned increasing values to increasingly detailed output. The program's internal calls use INFO or DEBUG, never logOff as a record level. Use one of the three supported configuration values.

The leading `!` in `if (!shouldLog(logDebug))` means NOT. `return;` exits the logger early, leaving hardware behavior to continue. Logging functions report behavior; they do not own the LED or cycle counter.

## New arithmetic: remainder

`completedCycles % summaryEveryCycles` is the remainder after integer division. For a divisor of five, counts 5, 10, and 15 have remainder zero. This gives occasional summaries without printing one every cycle.

The expression checks `summaryEveryCycles > 0` first. C++ short-circuit evaluation of `&&` skips the second expression when the first is false, preventing modulo by zero. A summary interval of zero deliberately disables progress summaries; it does not stop the LED or DEBUG records.

## Trace the baseline

With a ready monitor from the beginning, output starts with:

```text
level=INFO event=diagnostics_started lesson=02 exercise=02
level=INFO event=config on_ms=200 off_ms=800 summary_every_cycles=5 log_level=2
level=DEBUG event=led_changed completed_cycles=0 led=ON
level=DEBUG event=led_changed completed_cycles=0 led=OFF
```

After the fifth complete on/off interval:

```text
level=INFO event=progress completed_cycles=5
level=DEBUG event=led_changed completed_cycles=5 led=ON
```

The first ten cycles produce 20 DEBUG lines and two progress lines, plus two initial INFO lines: 24 lines total if the monitor receives the whole run. Late connection changes the received count. USB buffering and host display timing are not precise measurements of LED edges.

## Substantial practice tasks

1. Trace the first five cycles before uploading. Capture the initial configuration and at least two summaries. Compare ordering with the trace above.
2. Upload separately with DEBUG, INFO, and OFF. Predict and record which record types remain. Verify the LED pattern continues in all three cases. Compare ten-cycle line counts under an ideal fully connected trace.
3. Change the timing to 400 ms on and 600 ms off, and the summary interval to three. Before uploading, calculate period, duty cycle, and summary spacing. Confirm the config record reflects your constants rather than stale text.
4. Set the summary interval to zero and test DEBUG mode. Explain why transition logs remain and no modulo-by-zero operation occurs. Restore a positive interval.
5. Close the monitor for several cycles and reopen it. The LED and counter continue. The announcement is not repeated once diagnosticsStarted is true; explain why a reopened monitor may lack configuration context. Reset and observe the announcement being attempted again. USB reconnection can still lose individual records.
6. Extend each progress summary with `led=OFF`, `on_ms`, and `off_ms` fields. Confirm the LED really has been commanded OFF at that call site. Reuse configuration values; do not hardcode duplicate numbers in the logger.
7. Add a separate startup field identifying your firmware revision, such as `firmware=lesson02_ex02_v1`. Change it after your extension and explain why version information helps compare logs from different uploads.
8. Restore your chosen log level and valid settings. Save a short representative trace, your output-volume calculations, and results in the notebook.

## Completion check

Explain configuration versus events versus state, INFO versus DEBUG filtering, why diagnostics_started is not a boot timestamp, and how short-circuit evaluation protects the remainder operation. Explain why a DEBUG line confirms the software issued an output command rather than independently measuring the physical LED.

State remains in RAM and restarts on reset. The unsigned cycle counter eventually wraps after its type's maximum; progress summaries around that boundary need not be evenly spaced. We are not treating it as persistent lifetime telemetry.

Logging adds work and can block depending on buffering and connection behavior. The program still uses delay; filtering output is not a guarantee of exact timing. Timestamps and logging overhead are the next exercise.

Status: source and guide prepared; target compilation and hardware/USB verification are pending.
