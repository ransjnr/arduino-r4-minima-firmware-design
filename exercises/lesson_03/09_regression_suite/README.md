# Exercise 09 — Reusable regression workflow

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex09_v1`. Sketch: `exercises/lesson_03/09_regression_suite/09_regression_suite.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal

Combine host and optional USB checks while keeping their evidence separate. A host-only pass must explicitly say device tests were not run. Reports retain command output and distinct blocked/fail outcomes.

From the project root:

```bash
python3 exercises/lesson_03/tools/regression.py --output-dir build/lesson03/ex09-host
```

For a connected-device run, first upload this sketch and close Serial Monitor:

```bash
.venv/bin/python exercises/lesson_03/tools/regression.py --port /dev/ttyACM0 --expected-firmware lesson03_ex09_v1 --output-dir build/lesson03/ex09-full
```

The wrapper runs host parser checks and, only with both explicit port and identity, the smoke suite. Each subprocess has a finite timeout. No build/upload or physical LED verification is implied. summary.json links the layers through the selected output directory; individual JSON files contain detailed evidence. Existing files at that output location are overwritten, so choose a distinct directory to retain a new run.

## Implementation and experiments

1. Read the wrapper and predict outcomes for host-only, correct board, wrong firmware, and missing dependency. Confirm no device code runs without a port argument.
2. Run host-only and inspect device_note. Explain why it cannot be called complete firmware acceptance.
3. Upload the correct sketch, run the full workflow, and inspect each layer. Record manual LED observations separately.
4. Mutate one parser copy's lower bound to 51. Predict the failed host layer. Run, retain the failing report, restore, and rerun in a new directory.
5. Run with incorrect expected identity and retain blocked evidence. Do not rewrite it as a successful test because the hardware was connected.
6. Extend the workflow with a required version metadata file or named run identifier. Keep failures visible, avoid auto-uploading an unexpected sketch, and preserve finite timeouts.
7. Select a compact board regression set from your boundary matrix and add at least two independent assertions to smoke. Explain why each covers a previously untested requirement.
8. Rerun after restoring all mutations and write a release-style result: host outcome, device outcome, manual observations, unresolved checks. Use pass/fail/blocked/not-run precisely.

## Completion check

Explain reproducible commands, fixture identity, independent layers, retained artifacts, regression detection, and why a green subset cannot hide a blocked required layer. Version control source and concise conclusions; generated build reports remain gitignored.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
