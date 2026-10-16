# Exercise 07 — Timing and blocking responsiveness

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex07_v1`. Sketch: `exercises/lesson_03/07_responsiveness/07_responsiveness.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal

Measure a reproducible workload and explain exactly what the measurement includes. This sketch keeps blocking blink behavior. A query behind blink cannot be handled until the waits and preceding response output finish.

Upload this exercise, close Serial Monitor, and run from the project root (dependency setup is in Exercise 06):

```bash
.venv/bin/python exercises/lesson_03/tools/device_checks.py --port /dev/ttyACM0 --expected-firmware lesson03_ex07_v1 --suite timing --output build/lesson03/ex07-timing.json
```

The runner resets application state, sets 200/800, sends the short pipeline `blink` then `status`, waits for start/finish/status in order, and records host monotonic elapsed time. It intentionally sends only a few bytes, not unlimited input while the handler blocks.

## Experiments

1. Draw the timeline: host send, device input collection, blink start log, ON wait, OFF wait, finish log, status capture, USB receive. Mark the interval the runner measures.
2. Predict nominal waits of 1000 ms. Run at least five separate measurements with separate report filenames; calculate min, max, and mean. Do not present host round-trip time as precise command latency or electrical period.
3. Extend timing(session) to accept a pair of explicit durations with validation matching the protocol. Compare 100/100, 200/800, and 1000/1000. Adjust response deadlines conservatively for the maximum test duration; never remove deadlines.
4. Add a timestamp to blink_started and blink_finished in a working copy, capture before printing, and compare board intervals with host intervals. Update firmware revision and expected identity together.
5. Explain why a timestamp taken while preparing status is not the instant it arrived over USB. Use evidence, not subtraction of unrelated host and board clock origins.
6. Repeat while the monitor is closed as required. If another app owns the port, record blocked setup rather than diagnosing firmware timing.
7. Write a responsiveness requirement that the blocking design fails, such as handling stop within 100 ms during a two-second action. Do not implement a fake pass by acknowledging before taking the requested action. Carry the requirement into the nonblocking lesson.

## Completion check

Explain nominal delay, observed board interval, host round trip, clock resolution, buffering, and scheduling variation. Document workload and sample count. The later nonblocking phase changes scheduling; this exercise establishes the baseline honestly.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
