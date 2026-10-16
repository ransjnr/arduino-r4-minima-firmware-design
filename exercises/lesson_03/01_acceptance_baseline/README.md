# Exercise 01 — Establish an acceptance baseline

## Goal

Replace “it seems to work” with repeatable evidence. Define a starting state, exact stimulus, expected software response, physical observation, and pass/fail rule before running a test. This exercise reuses a stable manual indicator so you can focus on test design rather than adding features.

The firmware comes from Lesson 02's categorized-error example, with a version query added. There are no automatic patterns. Previous exercise files are unchanged.

## Protocol under test

Commands: help, version, status, errors, clear errors, on, off, blink, set on N, set off N. Numeric duration range is 50..5000 ms inclusive; default timings are 200/800. Matching is exact and case-sensitive. Maximum line content is 31 printable ASCII bytes. LF/CR/CR+LF terminate input; blank lines are ignored. An oversized/invalid line is discarded until its terminator and counted once.

A completed blink ends off and increments completed_blinks once. A rejected command preserves settings and LED state. status/errors/version are read-only with respect to application state. clear errors modifies only rejection history. Counters and configuration are RAM values reset by a board restart.

## VS Code workflow

This sketch is now selected in `.vscode/arduino.json`. Verify/upload using the existing Minima configuration. Close other serial monitors before upload. Open your selected board port at 115200 and set **Newline/LF**. Request version and confirm `lesson03_ex01_v1` before collecting results.

Press the board reset button before each independent full test run; reconnect the monitor if USB changes. Do not assume opening the monitor resets the program. Do not mistake clear errors for restoring all defaults.

## New vocabulary

- Requirement: an observable rule the firmware must satisfy.
- Test case: starting conditions, exact input, expected result, and pass/fail rule.
- Oracle: the rule or expected result you compare against, defined before looking at actual output.
- Test fixture: the controlled setup, including board, firmware version, port, and monitor settings.
- Regression: behavior that used to work but breaks after a change.

## Work through it

1. Read [the acceptance cases](acceptance-cases.md) before uploading. Calculate the expected final counters yourself and explain the contribution of each rejected line.
2. Verify/upload, record version, board/core version if known, date, and monitor settings in [the run record](run-record.md). Compilation success is only the build check, not acceptance.
3. Run the cases in order from a physical reset. Send each command separately and wait for its reply before continuing. The command file is a checklist, not a script to paste in one burst: receive buffering is finite and blink blocks.
4. Record actual replies and pass/fail after every case. Compare stable field values; captured_at_ms varies and is not an exact expected constant.
5. Observe LED behavior separately from text. The program can print a command even if the LED hardware fails; status reflects software's commanded state.
6. Repeat the entire run from reset without changing the code. Compare outcomes and document why timestamps differ even when both runs pass.
7. Add two test cases of your own: one for an accepted boundary duration, and one for an overlong line with a valid-command suffix. Define expected counters before running. Run these as a separate reset-based test so the reference sequence's totals remain valid.
8. Make one deliberate change in your working copy: alter the default ON time from 200 to 300 and update firmwareRevision to v2. Identify which baseline case should fail before uploading. Run it and record the detected regression, then restore defaults/revision and rerun. Keep the original requirement unchanged for this experiment.
9. Explain any failed or blocked cases. Do not mark a test passed because a neighboring case passed. Preserve your evidence and final restored sketch.

## Completion check

Explain why version identity, reset preconditions, exact test order, explicit expected results, and physical checks matter. Distinguish test failure (observed mismatch), blocked test (setup prevents execution), and not run. State what this baseline does not establish: precise electrical timing, indefinite uptime, arbitrary USB traffic, or responsive handling during delay.

Status: source and acceptance specification prepared. Physical board acceptance and target build/upload still need evidence.
