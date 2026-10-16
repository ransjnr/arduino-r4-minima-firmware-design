# Exercise 03 — Elapsed time and measurement limits

## Goal

Timestamp LED commands with the board's own clock, calculate intervals, and investigate how logging affects the program being observed. Keep blocking delays for now; millis is a measurement tool here, not a new scheduler.

Open the sketch, verify/upload for UNO R4 Minima, and open Serial Monitor at 115200. No extra hardware is required.

## Clock readings versus durations

`millis()` returns an unsigned long count of milliseconds since the program started. It is not a calendar date or a PC timestamp. A duration is the difference between two readings:

```cpp
const unsigned long observedOnMs = offAtMs - onAtMs;
```

Capture the reading immediately after the output command, before printing. Store it and print that saved value. Calling millis separately for every output field would describe different instants.

The timestamp approximates when the program issued a command. It does not independently measure a physical electrical edge or emitted light. Millisecond resolution also cannot reveal very small timing costs.

## What this sketch measures

| Field | Meaning |
|---|---|
| t_ms in led_changed | Clock captured immediately after the corresponding output command |
| observed_on_ms | OFF-command timestamp minus this cycle's ON-command timestamp |
| previous_on_to_this_on_ms | This cycle's ON timestamp minus the preceding cycle's ON timestamp |
| t_ms in cycle_finished | Clock after both configured waits, before printing the summary |

The first cycle reports `NA` for the period because there is no previous ON event. This is missing evidence, not a zero-length cycle. The period in a cycle-finished summary belongs to the interval ending at that cycle's ON event, not at the summary's timestamp.

A zero-overhead illustrative trace is:

```text
level=DEBUG event=led_changed t_ms=0 completed_cycles=0 led=ON
level=DEBUG event=led_changed t_ms=200 completed_cycles=0 led=OFF
level=INFO event=cycle_finished t_ms=1000 completed_cycles=1 observed_on_ms=200 previous_on_to_this_on_ms=NA
level=DEBUG event=led_changed t_ms=1000 completed_cycles=1 led=ON
level=DEBUG event=led_changed t_ms=1200 completed_cycles=1 led=OFF
level=INFO event=cycle_finished t_ms=2000 completed_cycles=2 observed_on_ms=200 previous_on_to_this_on_ms=1000
```

Real first timestamps need not be zero and real intervals include overhead. DEBUG output happens after each command and before its wait. Thus printing the ON record can lengthen the ON interval. The cycle summary prints while the LED is off, so it can lengthen the off gap and next ON-to-ON period. INFO emits no edge records but still emits summaries. OFF disables all logs but continues clock capture and LED operation.

## Unsigned subtraction and rollover

On the Minima, the unsigned long millisecond counter wraps after about 49.7 days. Use the same unsigned type for both readings and subtract later minus earlier. Unsigned arithmetic gives the correct elapsed duration across one wrap when the actual interval is less than the clock's full wrap period. It cannot tell you how many complete wraps occurred.

Paper example with a 32-bit counter: earlier = 4,294,967,290 and later = 14 after wrapping. The elapsed duration is 20 ms. Do not clamp this subtraction to zero just because the later numerical reading is smaller. This deliberate modulo arithmetic differs from accidentally underflowing a configured delay in Lesson 01.

## Experiments

1. Trace two cycles on paper. Upload, then capture at least ten consecutive summaries and their edge records. Discard the first `NA` period when calculating period statistics.
2. For at least five valid measured periods, calculate minimum, maximum, and mean. Compare with nominal 1000 ms. Separately compare observed_on_ms with configured 200 ms. Do not claim an exact hardware timing guarantee from this sample.
3. Use INFO, then DEBUG in separate uploads with unchanged timing. Compare measured period samples. Differences may be small or below millis resolution; record that honestly rather than expecting a fixed overhead.
4. Enable Arduino IDE's host-side timestamp display if available. Compare it with t_ms. Explain why USB buffering, host scheduling, and display batching can make PC arrival intervals differ from board event intervals.
5. Temporarily insert `delay(50);` immediately after `logEdge("ON", onAtMs);`. Predict observed ON time and period. Capture output to test the prediction, then remove the artificial delay and retest. This creates a visible measurement effect without relying on a specific USB transfer cost.
6. Modify timing to 400 ms on and 600 ms off. Predict what stays the same and what changes in the measurements. Upload and compare at least five periods.
7. Work through the rollover paper example. Explain why `if (later >= earlier)` would discard a valid measurement at wrap. You do not need to leave the board running for weeks.
8. Extend the summary with `nominal_period_ms` using the configuration constants. Keep measured and configured quantities clearly named. Save your final sketch and representative output in the notebook.

## Completion check

Explain timestamp versus elapsed duration, event capture versus log delivery, why the first period is unavailable, where output changes timing, and the assumptions behind unsigned subtraction. Explain why calling millis does not make a program nonblocking while delays remain.

Status: source prepared; target compilation and physical/USB measurements are pending. This exercise measures software-observed intervals; precise electrical timing requires appropriate external instruments.

## Official reference

https://docs.arduino.cc/language-reference/en/functions/time/millis/
