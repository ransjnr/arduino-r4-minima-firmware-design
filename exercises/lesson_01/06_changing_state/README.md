# Exercise 06 — State that changes over time

## Goal and specification

Run three heartbeat patterns, then three triple-flash patterns, and repeat automatically. Each complete pattern takes nominally 2000 ms. A phase lasts 6000 ms and the six-pattern sequence lasts 12000 ms. No upload is needed between phases.

We use “completed burst” to mean one complete pattern, including its quiet interval. A heartbeat counts as one pattern; a triple flash also counts as one pattern, not three.

## State versus configuration

`burstsPerMode` is configuration: a constant specifying the number of patterns in each phase. `selectedMode` and `completedBursts` are state: variables recording where the program currently is. Their declarations outside functions initialize them once at program startup, and they retain updated values between calls to `loop()`.

In contrast, the local `pulseIndex` in `sendBurst` is initialized again whenever that function enters its for loop. A counter declared as `unsigned int completedBursts = 0;` inside `loop()` would restart at zero every call and would never reach three in this implementation. Local variables can also be declared `static` to persist, but we are using globals here so the distinction is visible.

## Trace the transition

| Call to loop | Pattern executed | Count after pattern | State for next call |
|---|---|---|---|
| 1 | Heartbeat | 1 | Heartbeat, count 1 |
| 2 | Heartbeat | 2 | Heartbeat, count 2 |
| 3 | Heartbeat | 3, then reset to 0 | Burst, count 0 |
| 4 | Triple flash | 1 | Burst, count 1 |
| 5 | Triple flash | 2 | Burst, count 2 |
| 6 | Triple flash | 3, then reset to 0 | Heartbeat, count 0 |

The update occurs after the complete pattern returns. The new mode takes effect on the next call to `loop()`. Resetting the counter on a transition makes it count patterns in the new phase rather than all patterns since boot.

`completedBursts++` adds one. `>=` means greater than or equal to. Assignment, such as `selectedMode = burstMode`, replaces the stored value.

## Read the guard

The initial condition handles invalid state or configuration:

- `==` tests equality; `!=` tests inequality.
- `||` means logical OR: either invalid condition is sufficient.
- `&&` means logical AND: a mode is invalid when it differs from both allowed modes.
- Parentheses group the mode checks.
- `return;` ends this call to `loop()` immediately. The Arduino framework calls it again; it does not shut down the board.

For an invalid mode, the program holds the LED off for this call and restores the starting state. A zero threshold remains invalid on every call, so it keeps the LED off until the source configuration changes and is uploaded again.

## Experiments

1. Trace the first six calls using the table, then verify/upload and observe two full sequences. Count complete patterns rather than individual flashes.
2. Set `burstsPerMode` to 1, then 4 in separate uploads. Predict phase and full-sequence durations each time. Record what you observe.
3. Reset the board halfway through a burst phase. Predict the starting mode and counter after reset. Check your prediction. Ordinary RAM state is not a record saved across resets.
4. In a temporary edit, move the declaration and initialization of `completedBursts` from global scope into the start of `loop()`. Use threshold 3. Predict why switching stops; observe, then restore the global declaration.
5. Temporarily remove `completedBursts = 0;` from the transition block only. Trace the next several calls on paper before a brief hardware check. Restore the reset afterward. Explain why, after reaching the threshold, the modes switch every completed pattern rather than every three. Do not leave this fault running as your final firmware.
6. Set `selectedMode` initially to 99. Confirm recovery to heartbeat. Separately test a zero threshold and explain why it cannot recover by changing state alone. Restore valid settings.
7. Extend the working design to use separate constants for phase lengths: two heartbeat patterns and four triple-flash patterns. Decide which threshold to check based on the current mode. Predict the phase lengths and full sequence, implement, then verify on hardware.

## Completion check

Explain configuration versus state, why the counter persists, when a transition happens, why the count resets, and how a board reset differs from a mode transition. Record the final code settings and observations in the notebook.

This exercise still blocks during each pattern; it does not respond to outside events while waiting. Nonblocking timing is a later phase.

Status: source prepared. Compilation, hardware observations, and the separate-threshold extension are not confirmed.
