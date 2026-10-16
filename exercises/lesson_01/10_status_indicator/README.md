# Exercise 10 — LED status indicator capstone

## Goal

Integrate Lesson 01 into a documented application: a one-time startup signature, three operational modes, automatic transitions, and defined invalid-state behavior. This is a simulated status sequence, not a measurement of actual device health.

The starter is a complete reference implementation to study and extend. Earlier experiments remain separate. You must still compile, run, explain, and verify this version yourself.

## Behavior specification

| Phase/mode | Signal | Repetitions before transition | Nominal phase duration |
|---|---|---|---|
| Startup | Three 100 ms on / 100 ms off pulses, then 1400 ms extra off | Once per program start | 2000 ms |
| Heartbeat | 100 ms on / 1900 ms off | 2 | 4000 ms |
| Activity | Three 150 ms on / 150 ms off pulses, then 1100 ms extra off | 3 | 6000 ms |
| Alert | Five 100 ms on / 100 ms off pulses, then 1000 ms extra off | 2 | 4000 ms |

Operational modes repeat heartbeat → activity → alert → heartbeat. Each pattern takes 2000 ms nominally. The operational sequence is 14000 ms, and the first complete sequence ends about 16000 ms after setup begins. Bootloader time and instruction overhead are excluded.

Invalid configuration (any zero phase count) holds the LED off and suppresses the startup signal. An invalid operational mode holds the LED off for 100 ms, resets mode and count to their starting state, and resumes heartbeat on the next loop call. It does not repeat setup's startup signal. A board reset does repeat setup and reinitialize state.

## New language feature: returned values

Earlier helpers performed actions and returned `void`. `configurationIsValid` and `modeIsValid` return `bool`: true or false. The `!` operator means logical NOT, so `!configurationIsValid()` is true when configuration is invalid.

`cyclesForMode` returns an unsigned integer used by the transition comparison. `return value;` supplies that value and ends the function. The function returns 0 for an invalid mode, but loop checks validity before generating a pattern or using that threshold.

## Read responsibilities before editing

- `pulseLed`: generate one pulse.
- `sendBurst`: repeat pulses a bounded number of times.
- Validation helpers: check configuration and mode.
- `showPattern`: produce one complete operational pattern.
- `cyclesForMode`: select a mode's repetition threshold.
- `advanceMode`: choose the next mode.
- `setup`: initialize and show startup once.
- `loop`: validate, run a pattern, update progress, and transition.

The state consists of selected mode and completed cycles. The counter updates after the entire pattern finishes and resets at a mode transition. Delays and repetition counts are fixed positive values; the progress counter resets before it can grow without bound in normal operation.

## Implementation and verification tasks

1. Trace startup and the first seven operational cycles. Predict the mode, count, and nominal elapsed time at every boundary.
2. Verify/upload the unchanged reference. Observe startup and at least two full operational sequences. Record each mode's flash count and number of repetitions.
3. Calculate duty cycle for each operational mode and for the entire 14-second operational sequence. Weight total on time across all repetitions; do not simply average the three percentages.
4. Press reset during activity and again during alert. Confirm startup repeats and operation restarts from heartbeat. Record both cases.
5. Test an initial mode of 99. Predict startup, recovery, and first operational pattern in order. Restore valid state. Separately set one phase count to zero, confirm the LED-off behavior, then restore and upload valid configuration.
6. Refactor the literal timing values into named constants without changing behavior. Compare your revised code against the specification table and repeat the baseline observation.
7. Extend the program with maintenance mode: two pulses of 250 ms on and 250 ms off plus 1000 ms extra off; one complete cycle before returning to heartbeat. Add a mode identifier, repetition threshold, validation, pattern, threshold selection, and transition. Update the behavior table and calculate the new full sequence duration before uploading.
8. Retest all modes, invalid mode, invalid configuration, and reset. Record final code settings, observations, and limitations in the notebook.

## Hardware acceptance record

Use the notebook to mark each check pass/fail with evidence: startup once; correct heartbeat count; correct activity count; correct alert count; sequence order; invalid mode recovery; invalid configuration off; reset restarts startup; maintenance extension if implemented. Approximate visual timing is not precise measurement.

## Explain the limitations

This is a blocking design. A complete pattern must finish before loop evaluates state again. No sensor, user command, or real fault drives the mode transitions. Startup's signal is an application convention, not proof that all hardware is healthy. Later lessons introduce serial observability and nonblocking timing to improve interaction and responsiveness.

## Lesson 01 completion

Explain build versus upload; setup versus loop; constants versus changing state; parameter versus argument; local versus global lifetime; comparison versus assignment; repetition boundaries; clamping; and reset behavior. Use this program to point to an example of each applicable concept. Keep your calculations and actual hardware evidence alongside the code.

Status: reference source prepared; target compilation, hardware acceptance, refactoring, and maintenance extension are not yet confirmed.
