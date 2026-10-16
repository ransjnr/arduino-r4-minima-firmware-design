# Exercise 08 — Configuration fault injection and recovery

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex08_v1`. Sketch: `exercises/lesson_03/08_fault_recovery/08_fault_recovery.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal and fault contract

Distinguish rejected user input from corrupted application configuration. This sketch adds testFaultCase (0 normal, 1 inject ON duration zero at boot), a configurationFault flag, and a range guard. The source ships with injection disabled.

The guard detects timings outside 50..5000, commands LED off, and reports `event=fault reason=invalid_configuration` once per fault episode. While faulted, on and blink are blocked without incrementing input-rejection counters. status adds config_fault=1. Diagnostics, off, validated setting repairs, and test reset remain available. Once both settings are valid, the guard clears the flag and emits recovered once. Rejection statistics remain reserved for malformed user requests.

Detection is performed at command handling and after a received byte, not in a high-priority safety interrupt. The guard therefore does not claim immediate physical fault protection or detection of failed hardware. This is a controlled software-state experiment.

## Experiments

1. With selector 0, verify/upload and run the normal smoke suite using expected firmware `lesson03_ex08_v1`. This is the regression baseline, not a test of injected recovery.
2. Set testFaultCase to 1 and update firmwareRevision to `lesson03_ex08_fault_v1`. Upload. Request status and errors manually. Expect ON timing 0, config_fault=1, LED off, and zero input errors. A fault record can appear before the query reply.
3. Send on and blink. Confirm blocked responses, no completed blink, and LED off. Query repeatedly and explain why the fault is not logged on every received byte once latched.
4. Send set on bad. Expect a numeric rejection and configuration still faulted. Send set on 400; expect its acknowledgement and recovered after receive handling. Query status to verify config_fault=0, preserved OFF duration, and retained input-error count.
5. Test a normal blink after repair and confirm completion. Explain why valid commands alone did not repair the original invalid duration until a setting actually changed it.
6. Physically reset with selector 1 still compiled in. Confirm injection recurs. Compare with test reset, which restores valid state without boot injection. This difference is essential evidence.
7. Restore selector 0 and normal firmware revision, upload, and rerun smoke. Keep the final source in normal mode.
8. Extend the injection selector to corrupt OFF duration instead, then test detection and recovery. Do not bypass validation to repair production settings or add unbounded waits.

## Completion check

Explain input error versus configuration fault, single-episode logging, blocked actions, recovery conditions, and the limits of a polling guard. Record injected and restored firmware identities separately.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
