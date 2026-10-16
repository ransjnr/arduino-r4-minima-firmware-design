# Exercise 05 — Selectable LED patterns

## Goal

Build one firmware program with three selectable behaviors. Learn comparisons, branching, and a defined fallback. Reuse the pulse helper and bounded repetition from earlier exercises.

No serial commands or external inputs are used yet. Change `selectedMode` in the source and upload to select a mode. It is a variable that could change, but this version never changes it at runtime. Resetting starts it at its initial value again.

## Behavior specification

| Mode | Value | Behavior | Nominal repeating period | On time per period |
|---|---|---|---|---|
| Heartbeat | 1 | 100 ms on, 1900 ms off | 2000 ms | 100 ms |
| Burst | 2 | Three pulses, each 150 ms on and 150 ms off, then 1100 ms extra off | 2000 ms | 450 ms |
| Alert | 3 | Five pulses, each 100 ms on and 100 ms off, then 1000 ms extra off | 2000 ms | 500 ms |
| Invalid | Any other value | Explicitly hold LED off; wait 100 ms per loop call | No flashes | 0 ms |

For burst and alert, the last pulse's off interval comes before the extra pause. Distinct visual patterns can have the same total period. Timing is nominal; instruction execution adds overhead. Invalid mode's 100 ms wait is not a flashing period.

## Understand conditions

```cpp
if (selectedMode == heartbeatMode) {
  pulseLed(100, 1900);
} else if (selectedMode == burstMode) {
  sendBurst(3, 150, 150);
  delay(1100);
} else {
  // Other branches omitted here for explanation.
}
```

`==` compares two values and produces true or false. `=` initializes or assigns a value. They are different operations. Writing `if (selectedMode = heartbeatMode)` would assign the value 1; the condition then evaluates as true, selecting the wrong behavior. A program can compile and still be logically wrong.

An `if` / `else if` / `else` chain checks conditions in order. After one condition succeeds, it executes that branch and skips the remaining branches. If none match, it executes the final `else`. The full sketch includes an alert branch before the fallback.

The mode names are constants. `selectedMode` is a changing-capable variable. The function `sendBurst` now receives pulse count and timing arguments, so both burst and alert can reuse it with different settings.

## Work through the program

1. Trace execution with heartbeat selected. Name each function called and its arguments. Verify/upload and observe at least three cycles.
2. Set `selectedMode` to `burstMode`, upload, and count the flashes. Repeat with `alertMode`. Record each result separately.
3. Calculate each valid mode's duty cycle using on time / total period × 100. Equal periods do not imply equal duty cycles.
4. Set `selectedMode` to 99 and upload. Confirm the LED remains off. Restore a valid mode and upload again to distinguish intended off behavior from a board that cannot run the program.
5. In a temporary edit, change the first comparison from `==` to `=` and select burst mode. Predict and observe which pattern runs. Restore `==` immediately afterward. Explain the fault before continuing.
6. Extend the code yourself: add `doubleFlashMode = 4` and its branch before the final `else`. Generate two pulses with 100 ms on and 200 ms off each, then enough additional pause for a 2000 ms total cycle. Reuse `sendBurst`. Calculate the pause before coding it.
7. Test all four valid modes and invalid mode again. Keep the fourth mode implementation in your final sketch and record which mode you leave selected.

## Completion check

Explain comparison versus assignment, why only one branch runs, how the fallback is reached, and why changing a source variable requires another upload in this exercise. Explain what must change when a new mode is added.

This is compile-time configuration by source editing, not interactive mode selection. Runtime selection will come later. Delays still block progression through our sketch.

Status: starter source prepared; compilation, hardware behavior, and the fourth-mode extension are not yet verified.
