# Exercise 08 — Categorized errors and recovery

## Goal and contract

Extend the working line protocol with useful rejection statistics. A failed command must leave settings and commanded LED state unchanged and allow the next complete valid command to succeed.

Keep help, status, on, off, blink, set on N, and set off N. Add:

| Command | Result |
|---|---|
| errors | One read-only record containing total, category counts, and last error |
| clear errors | Explicitly reset only rejection counters and last error |

The exact clear command contains a space. Underscore labels in the help summary are shorthand, not command spellings; the usage line gives the syntax.

| Category | Trigger |
|---|---|
| unknown | Unknown complete command |
| numeric | Empty/non-digit numeric token after a recognized setting prefix |
| range | Digit-only value outside 50..5000 |
| line | Oversized or nonprintable/non-ASCII line, discarded to its terminator |

A blank line or CR+LF tail is not a rejection. One invalid complete line increments the total and exactly one category. With no wrap, total equals the sum of category counters. The fixed last-error label remains until another rejection or clear/reset; valid commands do not erase it. Counters are unsigned RAM values, wrap eventually, and reset with the board.

## VS Code workflow

Select this sketch when reaching Exercise 08: set the sketch field in `.vscode/arduino.json` to `exercises/lesson_02/08_error_handling/08_error_handling.ino`. Board and port settings stay as configured. Verify/upload, open Serial Monitor at 115200, select Newline/LF, and send help. Close the monitor before upload.

## Design reasoning

A single reportError helper owns classification, counting, and error output. Query functions do not increment counters. clearErrors is intentionally a modifying command, unlike errors and status. It must not call setup or reset application configuration as a shortcut.

lastError points to fixed string literals supplied by the callers, not temporary text inside the receive buffer. Retaining a pointer to that mutable receive buffer would allow the next command to corrupt diagnostic history.

Input validation errors describe rejected user requests. A corrupted program configuration or a disconnected physical peripheral is a different failure class; this sketch does not claim to diagnose hardware health. It continues with its last validated settings after bad input rather than rebooting.

## Substantial experiments

1. Trace an unknown command, malformed number, out-of-range number, and discarded line through reportError. Predict category counts before running.
2. Verify/upload and send clear errors, then status and errors. Confirm zero counts and last=none, with no LED change.
3. Send x, set on bad, set off 49, and a line containing 32 x characters, each newline-terminated. Request errors. Expect total=4 and one in each category; last=line_rejected.
4. Send on, set on 400, set off 600, then errors and status repeatedly. Confirm accepted actions do not increase or clear rejection counts, and read-only queries do not change application state.
5. Send an oversized line ending in on while the LED is held off. Confirm no suffix action and one line rejection. Send on as a fresh line to prove boundary recovery.
6. Clear errors after changing timings and completing a blink. Use status to prove timings, LED state, and completed blink count are preserved while rejection counts clear.
7. Test blank lines and CR/LF/CR+LF after clearing. Confirm no spurious errors or double counts.
8. Extend error records to include total and the current board timestamp. Capture the timestamp before printing. Verify the counters still update once per rejection and queries stay read-only.
9. Write a reproducible ten-command acceptance sequence with predicted final counters and application state. Run it from reset, compare evidence, and retain the sequence in the notebook.

## Completion check

Explain rejection versus hardware fault, aggregate versus category statistics, fixed-label lifetime, recovery at a line boundary, and explicit reset versus read-only querying. Show how a failed setting preserves the last valid value.

Status: source prepared; target build/upload and physical USB verification are pending. Delays still block during blink, and input buffering remains finite.
