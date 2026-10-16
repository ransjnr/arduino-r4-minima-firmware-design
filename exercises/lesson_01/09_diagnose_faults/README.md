# Exercise 09 — Diagnose deliberate faults

## Goal and reference behavior

Practice debugging logically incorrect firmware that still compiles. The reference is three pulses, each 200 ms on and 200 ms off, plus an 800 ms group pause. The nominal group period is 2000 ms, with 600 ms on time and 30% duty cycle. The final continuous off interval is 1000 ms.

The sketch includes baseline case 0 and three isolated, deliberate defects selected by `faultCase`. These are teaching examples, not intended production behavior. Select one at a time. Earlier exercise sketches remain unchanged.

## Debugging method

For each case, write the expected behavior, predict the actual symptom from the code, run it, and record observations. State a hypothesis tied to one line. Change one cause, upload, and compare again. Finish by retesting baseline. A successful compile proves neither correct timing nor correct behavior.

| Case | Investigation | Evidence to gather |
|---|---|---|
| 0 | Establish a working reference | Flash count, visible grouping, calculated period |
| 1 | Duration/units error | Compare the supplied duration with the required 200 ms |
| 2 | Misplaced group pause | Trace which delay executes on every iteration |
| 3 | Repetition boundary error | List all counter values that satisfy the comparison |

## Experiments

1. Verify/upload case 0, observe several groups, and record the reference. Predict all three faulty cases before running them.
2. Select case 1 and upload. Very short flashes can be hard to see; visual observation alone cannot measure a 2 ms pulse accurately. Use the source trace to identify the incorrect duration. Correct that branch to use the configured on time, then retest it.
3. Select case 2. Calculate the time for each iteration and for three iterations. Explain why grouping is lost. Move the group pause outside the loop, then retest.
4. Select case 3. Trace the counter values and calculate the resulting pulse count and period. Correct the comparison and retest.
5. Select each corrected case in turn and confirm it matches case 0. Record separate results rather than assuming a fix worked because it compiled.
6. Return `faultCase` to 0. Change the reference count to five and choose an additional pause that gives a 3000 ms group period. Update the shared pause constant, then check that the corrected branches still follow the same configuration.
7. Keep the corrected sketch as your result. Document each original fault and its fix in the notebook so the debugging record remains available after the defects are removed.

## Completion check

For each defect, explain the cause, predicted symptom, observed symptom, minimal correction, and regression check. Distinguish an upload failure from a program that uploads successfully but behaves incorrectly. Identify what evidence would require a logic analyzer or oscilloscope rather than eyesight.

Status: baseline and deliberate-fault source prepared; target compilation, fixes, and hardware results are not yet confirmed.
