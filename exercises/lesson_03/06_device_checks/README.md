# Exercise 06 — Bounded connected-device checks

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex06_v1`. Sketch: `exercises/lesson_03/06_device_checks/06_device_checks.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal

Use a host serial runner to test the real uploaded firmware through its USB interface. It checks firmware identity and responses, records a transcript, and fails on finite timeouts. It never builds or uploads automatically.

This and subsequent normal test sketches add `test reset`: a fixture command restoring default application state and counters, with LED off. It is not a physical restart, does not test startup/persistence, and does not clear receive bytes queued behind itself. The runner sends commands one at a time after a completed line.

## Install the optional device dependency

Run from the project root in VS Code's terminal:

```bash
python3 -m venv .venv
.venv/bin/python -m pip install -r exercises/lesson_03/tools/requirements.txt
```

The virtual environment and reports are gitignored. If venv support is missing, install your distribution's Python venv package before retrying; do not use system-wide sudo pip.

Verify/upload this sketch, close all serial monitors, and identify the current port. Replace the example port if yours differs:

```bash
.venv/bin/python exercises/lesson_03/tools/device_checks.py --port /dev/ttyACM0 --expected-firmware lesson03_ex06_v1 --output build/lesson03/ex06-device.json
```

## What it checks

Defaults; on and read-only status; accepted settings; numeric/range/unknown/line errors; failed-update preservation; one completed blink; clearing only errors. It finishes with test reset so the LED is commanded off. Counters in intermediate snapshots are checked before that cleanup.

## Implementation and experiments

1. Read device_checks.py. Locate write timeout, read deadline, bounded response length, version check, and expected fields. Explain why successful port opening alone is not fixture verification.
2. Run the tool on the correct uploaded sketch. Inspect raw transcript and outcome. Observe the LED separately; JSON explicitly does not mark physical LED verification passed.
3. Intentionally supply a wrong expected firmware name. Predict a blocked fixture result and confirm it. Restore the correct identity.
4. Close the board/choose an unavailable port and observe blocked rather than pass. Keep this separate from a response assertion failure.
5. Extend smoke with accepted setting endpoints and CR+LF input. Define expected state/counters before implementing. Keep single-command sequencing and finite timeouts.
6. Rerun twice and compare stable fields, allowing timestamps to differ. Record physical reset behavior manually outside the fixture reset command.

## Completion check

Explain build, upload, interface test, and physical observation as different evidence. Exit 0=interface checks pass, 1=assertion/response failure, 2=blocked fixture/dependency. Reports include whether hardware light output was actually observed; the tool cannot establish it.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
