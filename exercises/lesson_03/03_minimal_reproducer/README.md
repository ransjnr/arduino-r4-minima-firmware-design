# Exercise 03 — Reproduction and minimization

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex03_v1`. Sketch: `exercises/lesson_03/03_minimal_reproducer/03_minimal_reproducer.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal and seeded defect

This sketch intentionally changes a timing target to the minimum when a setting is rejected. The requirement remains: rejected settings preserve the last valid value. All other reference behavior is retained. Fixing the defect is your implementation task.

## Investigation

1. Read the requirement before inspecting the seeded-defect line. Physically reset, confirm version, then run: help; on; set on 400; set off 600; status; set on bad; errors; status; off. Predict the final ON setting and record the mismatch.
2. Remove irrelevant commands from a copy of that sequence, one at a time. Reset between independent attempts so history does not conceal preconditions. Verify the failure still reproduces after each reduction.
3. Try the candidate minimal sequence `set on 400`, `set on bad`, `status` from reset. State why the first command matters: without establishing 400, the fault could still be observed against default 200, so consider whether an even smaller sequence meets your chosen failure claim.
4. Define your claim precisely: “rejected update preserves explicitly configured value” or “rejected update preserves default.” Give a minimal reproducer for each and explain the difference.
5. Trace parseDuration, applySetting, and status. Identify which write violates the contract. Correct that write, update firmwareRevision to `lesson03_ex03_fixed`, and upload.
6. Run the retained reproducer and both-setting boundary checks. Also test a valid update afterward so removing the bad write has not removed all commits.
7. Restore a known fixture between runs, preserve before/after traces, and record whether the fault was detected by output, state query, or physical observation.

## Completion check

Explain minimal stimulus versus minimal precondition, why reset matters, why a changed version belongs in evidence, and why a rejected input can still have a side effect even when its error text looks correct. Do not copy this deliberately faulty sketch into later exercises; they use corrected behavior.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
