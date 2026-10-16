# Exercise 07 — Timing sequences and numeric boundaries

## Goal and specification

Generate pulses with on durations 500, 400, 300, 200, and 100 ms in that order. Every pulse includes a 300 ms off interval. After the minimum pulse and its off interval, wait another 1200 ms, then restart at 500 ms.

This changes pulse duration, not electrical brightness through PWM. We are still turning the built-in LED fully on and off.

## Trace the baseline

| Call | On duration | Off duration | Next state |
|---|---|---|---|
| 1 | 500 ms | 300 ms | 400 ms |
| 2 | 400 ms | 300 ms | 300 ms |
| 3 | 300 ms | 300 ms | 200 ms |
| 4 | 200 ms | 300 ms | 100 ms |
| 5 | 100 ms | 300 ms plus 1200 ms pause | 500 ms |

Total on time = 1500 ms. Normal off time = 5 × 300 = 1500 ms. Full sequence = 1500 + 1500 + 1200 = 4200 ms nominally, with approximately 35.71% duty cycle. The gap after the last falling edge is 1500 ms, because the regular off interval is included before the additional pause.

## Configuration, state, and invariant

The minimum, maximum, step, and pauses are configuration constants. `currentOnMs` is state: it changes after each pulse and persists between calls to `loop()`.

Our invariant is a condition we maintain: immediately before every pulse, `minOnMs <= currentOnMs <= maxOnMs`. We also require a positive minimum and a positive step. Invalid configuration holds the LED off. The range check restores an out-of-range state to the maximum before using it.

`currentOnMs -= stepMs` subtracts the step and stores the result. It is shorthand for `currentOnMs = currentOnMs - stepMs`.

## Why the subtraction needs a boundary check

Unsigned values cannot represent negative numbers. Subtracting a larger value from a smaller one wraps around to a large positive value. Feeding that result to `delay` could produce a very long wait.

We avoid this by checking the remaining distance to the minimum first:

```cpp
if (stepMs >= currentOnMs - minOnMs) {
  currentOnMs = minOnMs;
} else {
  currentOnMs -= stepMs;
}
```

The earlier range check guarantees that `currentOnMs - minOnMs` is not negative. If the step reaches or crosses the minimum, we clamp to the minimum instead of subtracting. Otherwise subtraction is safe and stays above the minimum. The minimum gets its own pulse on the next call, then the sequence restarts.

For example, with maximum 500, minimum 100, and step 150, the sequence is 500, 350, 200, 100. It must not continue to 50. The final step can be shorter than the configured step so that we include the exact minimum.

## Experiments

1. Trace the baseline table, verify/upload, and observe at least three full sequences. Record the nominal period calculation and observed order.
2. Change the step to 150. Predict every pulse duration and the full sequence duration before uploading. Explain the shortened final decrement.
3. Use a step larger than the entire range, such as 1000. Predict the pulse list before uploading. The boundary logic should still include both endpoints.
4. Set minimum and maximum both to 200 with a positive step. Predict the single-pulse sequence period. Verify that this is a valid configuration.
5. Test invalid configurations separately: zero step, zero minimum, and minimum greater than maximum. Predict the LED-off fallback. Restore valid values and upload after each test so that you confirm recovery.
6. On paper only, remove the boundary check and trace repeated subtraction with maximum 500 and step 150. Identify when it goes below the intended minimum and when unsigned wrap would occur. Do not upload that faulty version: a wrapped duration can cause an impractically long wait.
7. Design your own sequence with at least four pulse durations, a positive minimum, and a step that does not divide the range exactly. Predict the duration list, total on time, complete period, and duty cycle. Implement by changing constants, then compare the hardware behavior with your prediction.

## Completion check

Explain why the minimum pulse occurs before restarting, why the range check comes before computing the remaining distance, and why a zero step must be rejected. Distinguish clamping a value from allowing arithmetic to wrap. Explain why visual observations confirm order more reliably than exact millisecond timing.

The sketch uses blocking delays; instructions add some overhead to nominal timing. Compilation and hardware behavior remain unverified until you record them.
