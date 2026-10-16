# Exercise 03 — Build a repeatable signal

## Specification

Repeat two short flashes and one long flash. Short flashes last 200 ms, the long flash lasts 600 ms, gaps between flashes last 200 ms, and the final quiet gap lasts 1800 ms. Use only the built-in LED.

## Understand the new tool: a function

`pulseLed` groups four operations into a named action: on, wait, off, wait. Defining the function does not execute those operations. A call such as `pulseLed(shortPulseMs, pulseGapMs);` executes its body and then returns to the following statement in `loop()`.

`void` means the function returns no value. The parameters `onMs` and `offMs` are local names that receive the caller's argument values. On the first call they receive 200 and 200. On the third call they receive 600 and 1800. Each call has its own parameters; changing a parameter would not change the global constant supplied by the caller.

The global constants describe the pattern. The helper function describes how one pulse works. The three calls in `loop()` describe their order. Parameters let the same function generate different pulse lengths without repeating the hardware-control statements.

## Trace one cycle before uploading

| Time from cycle start | LED action |
|---|---|
| 0 ms | On: first short pulse |
| 200 ms | Off |
| 400 ms | On: second short pulse |
| 600 ms | Off |
| 800 ms | On: long pulse |
| 1400 ms | Off: final quiet gap |
| 3200 ms | Next cycle begins |

Total cycle = (200 + 200) + (200 + 200) + (600 + 1800) = 3200 ms. Total on time = 1000 ms. Duty cycle = 1000 / 3200 × 100 = 31.25%. These are nominal durations; code execution adds a small overhead.

The last call already includes the final off interval. Adding another `delay(signalGapMs)` afterward would lengthen that gap twice.

## Work through it

1. Read the sketch and trace the first call into the helper and back to `loop()`.
2. Verify and upload. Observe at least three complete patterns; distinguish the two short flashes from the long flash.
3. Change only constants to use 150 ms short flashes, 450 ms long flashes, 150 ms pulse gaps, and a 1250 ms final gap. Calculate the cycle duration and duty cycle before uploading. Check your calculation against the order of calls.
4. Modify `loop()` to produce long-short-short. Reuse the helper. Ensure the long quiet gap belongs after the final short flash, rather than after the long flash.
5. Restore the short-short-long pattern with your chosen constants. Record the final behavior and results in the notebook.

## Completion check

Explain the difference between a definition and a call, a parameter and an argument, and a global constant and a local parameter. Explain why `setup()` runs once while this pattern repeats, and why the sketch does not reach the next call while `delay()` is waiting.

Status: source prepared; compilation and hardware results remain to be recorded. No additional hardware or libraries are required.
