# Exercise 07 — Query state without changing it

## Goal and protocol

Add a read-only status command to the validated command interface. Prove that reporting does not turn the LED off, reinitialize settings, reset counters, or perform a blink. Keep all previous commands and input bounds.

| Command | Behavior |
|---|---|
| status | Report one snapshot of current configuration and application state |
| help | Report supported commands and duration range |
| on / off | Hold the LED on / off |
| blink | Run one blocking blink with current durations, finish off, increment completed_blinks |
| set on N / set off N | Validate and commit 50..5000 ms durations |

This application has one operating mode, manual: commands select actions and there is no automatic repeating pattern. Mode is a fixed descriptive field, not a new configurable setting. Configured blink timings still appear while the LED is held on or off; they do not describe the duration of a manual hold.

## VS Code workflow

The workspace selects `07_status_queries.ino`. Verify, upload, then open Serial Monitor on the board's current port at 115200 with **Newline/LF** selected. Close the monitor before upload. Send status; no startup greeting is required.

An illustrative initial response is:

```text
event=status captured_at_ms=2400 mode=manual led=OFF on_ms=200 off_ms=800 completed_blinks=0 rejected_commands=0
```

Your captured_at_ms will differ.

## Define what each field means

- captured_at_ms: board millis reading when the snapshot was taken, not PC arrival time or a calendar timestamp.
- mode: manual operation, not a health diagnosis.
- led: last commanded state, not physical feedback from the LED.
- on_ms/off_ms: current validated configuration for future blink commands.
- completed_blinks: fully finished blink commands since reset. The increment occurs after both waits, before blink_finished is printed.
- rejected_commands: rejected complete input lines since reset, including unknown commands, invalid settings, and discarded lines. One line produces at most one rejection. Blank lines and CR/LF tails do not count.

The counters are unsigned RAM values and eventually wrap; they are not permanent lifetime statistics. Reset restores counts to zero, default timings, and LED off.

## Snapshot versus modification

`printStatus` copies all reported values into local constants before printing. These capture the same logical state because this single-threaded sketch has no interrupt or other task updating application state during capture. The copies do not by themselves provide concurrency safety for a future multitasking design.

The function calls no setLed, delay, counter increment, or setting update. Query handling consumes serial input and clears the completed receive line as usual, and serial printing takes time. Read-only here means application configuration/state is unchanged, not that executing the query has zero cost or changes no receive-buffer variables.

An error helper centralizes rejection counting and output. Detailed error categories and recovery policies are next exercise; here we need only a consistent aggregate count visible through status.

## Experiments and evidence

1. Trace the startup values. Verify/upload and request status twice. Compare all fields: captured_at_ms can advance, but application fields must stay unchanged.
2. Send on, status, status, off, status. Verify the query does not change the commanded LED state. Record output and physical observations separately.
3. Send set on 500, set off 1500, status. Confirm both settings without running blink. Send blink, then status, and confirm completed_blinks increments once and the LED ends off.
4. Set a known valid configuration, then send set on 49 and status. Confirm one additional rejection, preserved settings, no additional completed blink, and unchanged LED state. Repeat with malformed numeric input.
5. Send an unknown command and a line longer than 31 content bytes, each with a terminator. Query after each and confirm one rejection per line. Test empty lines and CR+LF: neither should add a rejection.
6. Send blink, then status during its waits. Observe that the query responds only after blink finishes. It cannot report an intermediate blinking state in this blocking design. Timestamping the snapshot does not remove this limitation.
7. Build a short reproducible command sequence from a fresh reset with two completed blinks and three rejected lines. Predict final state, timings, and counters before sending. Capture the final status and explain any differences.
8. Extend status with nominal_blink_ms, computed from the captured ON and OFF durations. Do not use a hardcoded total and do not trigger a blink to calculate it. Verify after changing each timing independently and after a failed update.
9. Reset and reconnect if necessary. Confirm defaults and zero counters return. Keep the final query extension, test sequence, and representative output in the notebook.

## Completion check

Explain query versus command, configuration versus state versus statistics, software command state versus physical feedback, snapshot capture versus record delivery, and why a status query waits behind a blocking handler. Explain why repeated queries must not increment action counters.

Status: source prepared; actual target build/upload and USB/hardware observations remain pending.
