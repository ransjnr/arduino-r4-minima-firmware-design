# Exercise 10 — Verify a firmware change

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex10_v1`. Sketch: `exercises/lesson_03/10_verification_capstone/10_verification_capstone.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Change request

Add exact command `toggle` to the verified manual indicator. It must invert the commanded LED state once, reply `event=toggled led=ON/OFF`, preserve timings/error history/completed blink count, and leave all existing protocol behavior unchanged. Similar words such as `toggle now` must remain unknown commands. Reset still starts off.

The supplied reference implements this change with setLed(!ledIsOn), extends help, and updates its version. Your task is to verify it against explicit requirements and improve evidence, not merely upload the reference.

## Build and checks

Select this sketch, verify/upload, close Serial Monitor, then run from project root:

```bash
.venv/bin/python exercises/lesson_03/tools/regression.py --port /dev/ttyACM0 --expected-firmware lesson03_ex10_v1 --toggle --output-dir build/lesson03/ex10-reference
```

The wrapper runs host parser tests, the existing USB smoke suite, and a toggle-specific suite. Each suite establishes its own fixture with test reset, so smoke's final counters cannot leak into toggle expectations. No automatic upload occurs. Each successful suite ends by restoring application defaults with LED off.

## Verification work

1. Before running, create a requirement-to-test table. Include one toggle from off, one from on, two toggles restoring the initial state, unknown suffix rejection, and preservation of non-default settings and nonzero counters.
2. Read the supplied toggle suite. Identify missing coverage: its current preservation check starts with zero/default values. Extend it to establish 400/600, one completed blink, and a rejection before toggling, then prove all unrelated state is preserved.
3. Confirm help advertises toggle and that prior exact commands still work. Run host plus USB regression, then manually observe actual LED inversion; a text reply alone is not physical proof.
4. Review the code change relative to Exercise 09. Explain why setLed maintains commanded state and output together, and why directly flipping only the boolean would be insufficient.
5. Temporarily mutate toggle to setLed(true). Predict which assertion detects the regression, run it, retain failure evidence, restore the correct action, and rerun. Update firmware identity if retaining any distinct changed build.
6. Add a new test for CR+LF after toggle and for two complete toggle lines. Keep partial-line and overflow behavior unchanged.
7. Run the original reset-based acceptance baseline where applicable and document command/version differences rather than blindly expecting the old identity.
8. Write verification-report.md with version, exact commands used, requirement coverage, host results, full target build/upload evidence, USB outcomes, manual observations, unresolved failures/blocked checks, and remaining limitations. Link retained artifacts and explain which were not run.

## Completion gate

Do not claim overall verification complete while a required layer is missing. Separate prepared source, host pass, ARM syntax check, full Arduino build, successful upload, USB integration pass, and observed physical behavior. Counter wrap, arbitrary traffic, nonvolatile storage, and responsiveness during blocking blink remain outside this capstone's guarantee.

The next phase is Lesson 04: nonblocking timing and state machines. Carry forward a concrete responsiveness requirement from Exercise 07 instead of assuming input-buffer improvements made delay-based actions responsive.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
