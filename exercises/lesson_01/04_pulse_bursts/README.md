# Exercise 04 — Configurable pulse bursts

## Goal and specification

Generate three 200 ms flashes separated by 200 ms off intervals, followed by an additional 1400 ms quiet pause. Change the number of flashes through one constant, without copying pulse calls. Keep the helper function from Exercise 03.

The final pulse also includes its 200 ms off interval: the quiet time from the last falling edge to the next burst is 1600 ms. This distinction is part of the specification.

## Understand a for loop

```cpp
for (unsigned int pulseIndex = 0; pulseIndex < count; pulseIndex++) {
  pulseLed(onTimeMs, pulseGapMs);
}
```

The header has three parts separated by semicolons:

1. `unsigned int pulseIndex = 0` creates a local counter, initialized once on entering this loop. Unlike a constant, this variable changes.
2. `pulseIndex < count` is checked before every iteration. The body executes only while this is true.
3. `pulseIndex++` increases the counter by one after each completed iteration.

For `count = 3`, the body runs with counter values 0, 1, and 2. When the counter reaches 3, the comparison is false and execution continues after the loop. A zero-based counter still produces three flashes.

The counter exists only inside the for loop's scope. Every call to `sendBurst` starts a new counter at zero. The parameter `count` receives a copy of the argument supplied by the caller.

Arduino's `loop()` is a function repeatedly called by the framework. This `for` loop is a C++ control statement that performs a bounded repetition inside one call. They serve different purposes.

## Trace execution

`loop()` calls `sendBurst(3)`. The burst function executes three iterations. Each iteration calls `pulseLed`, waits through its on and off durations, and then increments the counter. After the burst function returns, `loop()` waits another 1400 ms. Then the framework calls `loop()` again.

Nominal burst-to-burst period:

`count × (onTimeMs + pulseGapMs) + burstPauseMs`

For the initial values: `3 × (200 + 200) + 1400 = 2600 ms`.

LED on time is `3 × 200 = 600 ms`, giving approximately 23.08% duty cycle. Function calls and other instructions add a small overhead to these nominal durations.

## Implementation and hardware experiments

1. Read the sketch, trace the three iterations on paper, then verify/upload. Observe at least three bursts.
2. Test pulse counts 1, 3, and 5. Before each upload, predict the total period and on time. Check the flash count by observation; visual timing checks are approximate.
3. Set the count to zero. Predict which parts still execute. Verify that the LED remains off and the extra pause still occurs. Restore a nonzero count afterward.
4. In a temporary edit, replace `< count` with `<= count`. Predict the extra iteration and verify the flash count. Restore `<` after observing the result. This is an off-by-one error.
5. Design a five-flash burst with 100 ms on and 100 ms off per pulse and a total cycle of 3000 ms. Calculate the additional pause, update the constants, and upload. Confirm the number of flashes and distinguish the final off interval from the additional pause.
6. Keep your final working settings and record all results in the lab notebook.

## Completion check

Explain why the counter starts at zero, why `<` differs from `<=`, what happens for zero pulses, and why adding pulses lengthens the cycle unless you adjust another duration. Explain which variables change and which values are constant.

The function has bounded repetition but still blocks while its delays run. We will address responsiveness in the nonblocking timing phase.

Status: prepared; no compilation or hardware execution has been confirmed for this exercise.
