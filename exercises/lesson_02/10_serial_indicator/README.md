# Exercise 10 — Serial-controlled indicator capstone

## Goal

Combine bounded line reception, strict numeric validation, queries, categorized errors, manual LED commands, and automatic patterns. Document and test a complete protocol. This application demonstrates status patterns; it does not measure hardware health.

Select `exercises/lesson_02/10_serial_indicator/10_serial_indicator.ino` in `.vscode/arduino.json` when ready. Verify/upload, then use the board's current port at 115200 with Newline. Close the monitor before uploading. The default after reset is manual mode with LED off, timings 200/800, and zero counters. No startup greeting is required.

## Protocol specification

| Command | Result |
|---|---|
| help | Commands and usage |
| status | Snapshot: mode, commanded LED, durations, completed one-shot blinks, automatic patterns, rejected lines |
| errors | Category counts and last rejection |
| clear errors | Clear only rejection history |
| on / off | Switch to manual mode and hold requested state |
| blink | Switch to manual, run one configured blink, finish off, increment completed_blinks |
| mode manual / stop | Switch to manual and command LED off |
| mode heartbeat | Select repeated configured ON/OFF pattern |
| mode burst | Select repeated three-flash pattern: 100/100 per pulse, then 400 ms extra off |
| set on N / set off N | Change configured durations, 50..5000 ms inclusive |

Exact case and spacing apply. LF, CR, and CR+LF terminate commands. Content capacity remains 31 bytes; nonprintable or oversized lines are discarded to their terminator. Invalid requests preserve settings, mode, and current commanded LED state at the time they are handled; ongoing automatic behavior can change the LED later.

A heartbeat pattern uses the configured durations, initially 1000 ms total with 20% nominal duty cycle. Burst uses fixed timings: 3 × (100+100) + 400 = 1000 ms, 300 ms on time, 30% nominal duty cycle. The final burst off gap is 500 ms. set commands affect heartbeat and one-shot blink, not burst timings. completed_patterns counts only completed automatic patterns across mode changes; completed_blinks counts only one-shot blink commands.

## Scheduling and response contract

Each loop call processes at most one byte. If more bytes are currently buffered, it returns for another receive pass before starting a new automatic pattern. Once the available queue is empty, one automatic pattern runs and blocks until complete. A partial line can remain pending while patterns continue if no further byte is currently available.

stop is a normal command, not an emergency interrupt. It cannot be handled inside a delay. With 5000/5000 settings, a heartbeat alone can occupy approximately ten seconds before input is processed again; USB/output overhead adds time. Repeated commands can consume receive capacity, and continuous input can postpone patterns. Neither guaranteed real-time response nor unlimited receive throughput is claimed.

status in automatic modes is processed between complete patterns, normally reporting LED off. It cannot observe intermediate blink state through a command while the handler is blocked. Queries do not change mode or application counters, but a subsequent automatic pattern may complete immediately afterward, making later queried counts larger.

## Architecture to trace

Receive bytes → finish a bounded line → exact dispatch → validate settings → commit or report an error. Query functions read application state. runAutomaticPattern owns automatic output and increments its counter only after a full pattern. Manual commands explicitly exit automatic mode to avoid a subsequent pattern immediately overriding the requested hold.

Mode names are fixed labels returned from modeName. A reference parameter updates validated settings, and a local candidate prevents partial updates. Error labels are fixed strings. No dynamic String allocation is introduced.

## Implementation and acceptance work

1. Write the command contract in your own words and trace reset, mode heartbeat, status, stop, then status. Distinguish command handling from the automatic work that can run afterward.
2. Verify/upload. Test help and default status. Test on, off, and one-shot blink, checking that each selects manual mode and preserves settings.
3. Set 500/1500 and enter heartbeat. Predict a 2000 ms pattern and 25% duty cycle. Observe multiple patterns. Send status and stop; record response behavior and final manual/off state.
4. Enter burst and observe three flashes per pattern. Change configured durations, then prove burst stays fixed and heartbeat uses the new settings. Stop and query counters.
5. Test all error categories. Confirm rejected settings preserve values, rejected mode words preserve selection, and an oversized line with stop as a suffix never becomes a valid stop command. Keep timings short for this test and send a fresh stop line afterward.
6. Complete a one-shot blink and multiple automatic patterns, then stop. Record counters before and after clear errors; action counts and settings must remain unchanged.
7. Set maximum heartbeat timings, enter heartbeat, and send stop during its wait. Record approximate delay honestly; do not interpret a delayed stop as a broken port. Restore shorter settings afterward.
8. Reset from each mode. Confirm default manual/off, restored timings, and zero statistics. Query again after any USB reconnection.
9. Extend the design with double-flash mode: two 150/150 pulses plus 400 ms extra off, nominally 1000 ms total. Update identifiers, names, dispatch, help, automatic output, and documentation. Ensure on/off/stop still exit every automatic mode.
10. Retest manual commands, every mode, queries, numeric boundaries, malformed input, CR/LF, oversized-line recovery, error clearing, and reset. Save firmware revision, final protocol, representative traces, timing calculations, and actual pass/fail evidence.

## Acceptance record

Use the notebook for separate results: baseline/defaults; manual commands; configured blink; heartbeat; burst; stop delay; query isolation; categories/preservation; buffer recovery; error-clear isolation; reset; optional extension regression. Source preparation and host simulation are not proof of a successful board upload or measured physical behavior.

## Explain the remaining limitations

This is intentionally blocking firmware. A responsive stop, simultaneous input handling, and consistent scheduling require replacing long delay-based work with incremental states. That is a later phase, not a guarantee of this capstone. The next lesson in the roadmap deepens testing and diagnosis; the nonblocking timing phase follows it.

Status: reference source prepared; actual target build/upload, hardware acceptance, and extension remain pending.
