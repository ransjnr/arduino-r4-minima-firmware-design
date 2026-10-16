# Exercise 09 — Reproduce and diagnose protocol defects

## Goal

Investigate software that compiles and accepts serial input but violates its behavior contract. Use evidence from status and errors rather than guessing. The sketch is a copy of Exercise 08 with three selectable deliberate defects. faultCase=0 is the reference; enable only one defect at a time.

Select this sketch in `.vscode/arduino.json`: `exercises/lesson_02/09_protocol_diagnosis/09_protocol_diagnosis.ino`. Verify/upload with the Minima selected, then open the monitor at 115200 with Newline. Close it before changing/uploads.

## Debugging method

For each defect, record a minimal reproducer, expected behavior, actual trace, hypothesis, single correction, and regression results. Start each independent test from reset or explicitly establish all relevant state; clearing errors does not reset settings or completed blinks.

| faultCase | Seeded defect | Contract violated |
|---|---|---|
| 0 | None | Establish the reference |
| 1 | Accept any command starting with on | Complete commands must match exactly |
| 2 | Change the setting when validation fails | Rejected updates must preserve valid state |
| 3 | Reset completed blink statistics during status | Queries must be read-only |

Deliberate faults are marked in source so you can inspect how they are introduced. Predict symptoms before running them. They do not introduce unbounded waits or memory writes outside the input buffer.

## Investigations

1. Run case 0. Establish LED off, timings 400/600, and one completed blink. Capture status and errors. Save this baseline before switching cases.
2. Enable case 1 and upload. Send off, then only, then errors and status. Predict whether the LED changes and whether the request is counted as rejected. Compare with reference behavior. Correct the on branch so it always uses exact strcmp matching; keep the fix and retest on, only, on with a trailing space, and off.
3. Enable case 2, upload, and explicitly set on duration to 400. Send set on bad, then status. Capture the error and the incorrectly changed setting. Remove the mutation in the error path. Retest malformed, out-of-range, and valid updates to both timing fields; confirm failed updates preserve state.
4. Enable case 3 and upload. Send blink, then status twice. Trace when the count is incremented and when the defect clears it. Remove the mutation in printStatus. Retest two blinks, repeated status, errors, and clear errors; only completed actions should change the blink counter.
5. After fixing all three branches, test every selector value 0 through 3 in separate uploads. The formerly faulty cases should now satisfy the same contract as the reference. Alternatively remove the fault machinery after documenting each fix, then run the full regression below. Keep your final choice explicit in the notebook.
6. Regression: test exact commands, settings at 50 and 5000, rejected 49 and 5001, malformed numeric input, CR+LF, oversized-line recovery, repeated queries, and clear-errors isolation. State what each test proves.
7. Add a short firmware revision identifier to help output so captures can identify your corrected build. Include the identifier in your final evidence.

## Interpreting evidence

An error message alone does not prove that rejected input preserved state: query the actual configuration afterward. A status record alone does not prove physical output: compare with the LED when practical. A query that reports the wrong counter can itself be the bug, so trace the update sites as well as reading its output.

## Completion check

Explain why successful compilation does not establish correct protocol semantics, why a minimal reproducer is useful, why state must be initialized between independent tests, and why fixes need regression checks of neighboring behavior.

Status: reference and deliberate-fault source prepared; actual target build/upload, learner corrections, and hardware observations remain pending.
