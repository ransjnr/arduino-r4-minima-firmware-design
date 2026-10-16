# Lab notebook

## Lesson 01 — Blink

- Date: 2026-10-16
- Prediction before changing the delays:
- Verify/compile result: Successful; reported 39,008 bytes of program storage and 3,940 bytes of global/static RAM use.
- Upload result: Initially failed with LIBUSB_ERROR_ACCESS. Upload worked after the Linux USB permissions fix, as reported by the learner.
- Observed behavior: Learner reported the Blink exercise and changed timing worked. Saved sketch currently contains 500 ms on and 500 ms off; that exact saved revision has not been separately confirmed on hardware.
- Explanation in my own words:
- Questions or errors: USB permissions issue resolved.

## Lesson 01 — Exercise 02: named timing values

- Status: Ready to start; not yet compiled or uploaded.
- Prediction:
- Values chosen for the exercise:
- Verify/compile result:
- Upload result:
- Observed behavior:
- Explanation in my own words:

## Lesson 01 — Exercise 03: reusable signal pattern

- Status: Source and guide prepared; compile/upload not yet confirmed.
- Baseline prediction: short-short-long; nominal 3200 ms cycle, 31.25% duty cycle.
- Verify/compile result:
- Upload result:
- Observed baseline behavior:
- Modified timing prediction and calculation:
- Modified timing observation:
- Long-short-short implementation and gap placement:
- Final chosen constants:
- Explanation of functions and parameters in my own words:

## Lesson 01 — Exercise 04: configurable pulse bursts

- Status: Prepared; compile/upload and hardware behavior not yet confirmed.
- Baseline: 3 pulses; nominal 2600 ms period, 600 ms on time.
- Verify/compile result:
- Upload result:
- Observed baseline:
- Count 1 — predicted period / observed flash count:
- Count 3 — predicted period / observed flash count:
- Count 5 — predicted period / observed flash count:
- Zero-count prediction and observation:
- Off-by-one prediction and observation (restore `<` afterward):
- Five-flash, 3000 ms design — calculated pause and observation:
- Final saved constants:
- Explanation of counter initialization, comparison, increment, and scope:

## Lesson 01 — Exercise 05: selectable LED patterns

- Status: Prepared; compile/upload and hardware behavior not yet confirmed.
- Verify/compile result:
- Heartbeat upload and observation:
- Burst upload and observation:
- Alert upload and observation:
- Calculated duty cycles:
- Invalid-mode upload and observation:
- Restored-valid-mode observation:
- Assignment-versus-comparison experiment (restore `==`):
- Fourth mode — calculated additional pause:
- Fourth mode — implementation and observation:
- Regression check: all four valid modes and invalid mode:
- Final selected mode:
- Explanation of branching in my own words:

## Lesson 01 — Exercise 06: changing state

- Status: Prepared; compile/upload and hardware behavior not yet confirmed.
- Baseline prediction: three heartbeat patterns, three triple-flash patterns; nominal 12-second full sequence.
- Verify/compile result:
- Upload and baseline observation:
- Threshold 1 — prediction and observation:
- Threshold 4 — prediction and observation:
- Reset during burst phase — prediction and observation:
- Local-counter fault — explanation and observation (restore global afterward):
- Missing-counter-reset fault — trace and observation (restore afterward):
- Invalid initial mode and zero threshold — separate observations:
- Two-heartbeat/four-burst extension — prediction, implementation, observation:
- Final settings:
- Explanation of persistent state in my own words:

## Lesson 01 — Exercise 07: timing sequences

- Status: Prepared; compile/upload and hardware behavior not yet confirmed.
- Baseline prediction: 500, 400, 300, 200, 100 ms on; nominal 4200 ms full sequence.
- Verify/compile result:
- Upload and baseline observations:
- Step 150 — predicted durations, period, and observations:
- Step 1000 — predicted durations and observations:
- Equal endpoints — predicted period and observation:
- Invalid configurations — separate results and restored-valid checks:
- Paper trace without boundary protection:
- Custom sequence — constants, duration list, period, duty cycle:
- Custom sequence — hardware observations:
- Explanation of clamping and unsigned arithmetic:

## Lesson 01 — Exercise 08: startup versus operation

- Status: Prepared; target compile/upload and hardware behavior not confirmed.
- Startup and first-operation timing prediction:
- Verify/compile and upload results:
- Startup and five operational cycles observed:
- Reset and power-cycle observations:
- Five-pulse startup calculation and observation:
- Misplaced-startup experiment and restoration:
- Custom startup signature, timing, and observation:
- Explanation of setup versus loop:

## Lesson 01 — Exercise 09: diagnose deliberate faults

- Status: Prepared; target compile/upload, corrections, and hardware behavior not confirmed.
- Baseline compile/upload and observation:
- Case 1 — expected, hypothesis, observed, correction, retest:
- Case 2 — expected, hypothesis, observed, correction, retest:
- Case 3 — expected, hypothesis, observed, correction, retest:
- Corrected-case regression results:
- Five-pulse/3000 ms configuration and observations:
- Final fault selector and configuration:
- Limits of visual measurement:

## Lesson 01 — Exercise 10: status indicator capstone

- Status: Prepared; target compile/upload, extensions, and hardware acceptance not confirmed.
- Startup and seven-cycle trace:
- Per-mode and overall duty-cycle calculations:
- Verify/compile and upload results:
- Startup once — pass/fail and evidence:
- Heartbeat count — pass/fail and evidence:
- Activity count — pass/fail and evidence:
- Alert count — pass/fail and evidence:
- Sequence order over two full sequences — pass/fail and evidence:
- Reset during activity and alert — pass/fail and evidence:
- Invalid-mode recovery — pass/fail and evidence:
- Invalid configuration off, then restored operation — pass/fail and evidence:
- Named-timing refactor and regression:
- Maintenance extension specification, sequence calculation, and observations:
- Final configuration and limitations:
- Lesson completion explanation:

## Lesson 02 — Exercise 01: serial observation

- Status: Prepared; target compilation and USB/hardware behavior not confirmed.
- Verify/compile and upload results:
- Monitor settings and port:
- Initial trace prediction:
- Ten consecutive output lines:
- Explanation of shared ON/OFF count:
- Modified timing prediction and observation:
- Monitor close/reopen observations:
- Reset/counter/USB observations:
- Added timing fields and sample lines:
- Counter-position experiment and restoration:
- Final timing and five sample lines:
- Explanation of reporting versus physical measurement:

## Lesson 02 — Exercise 02: structured diagnostics

- Status: Prepared; target compilation and USB/hardware behavior not confirmed.
- Verify/compile and upload results:
- Baseline trace prediction and captured output:
- DEBUG, INFO, OFF — predicted line types/counts and observations:
- Modified timing/interval — period, duty cycle, summary spacing, observed config:
- Zero-summary-interval test and restoration:
- Monitor reconnect versus board reset observations:
- Extended summary fields and sample output:
- Firmware revision field:
- Final settings and representative trace:
- Explanation of filtering and short-circuit evaluation:

## Lesson 02 — Exercise 03: elapsed time

- Status: Prepared; target build and hardware/USB measurements pending.
- Verify/compile and upload results:
- Two-cycle prediction and captured trace:
- Five or more measured periods; minimum, maximum, mean:
- Observed ON durations compared with configured durations:
- INFO versus DEBUG measurements:
- Host timestamp versus board timestamp observations:
- Artificial 50 ms delay — prediction, observation, removal/retest:
- 400/600 configuration — prediction and measurements:
- Rollover paper calculation and assumptions:
- Added nominal-period field and representative output:
- Final settings and measurement limitations:

## Lesson 02 — Exercise 04: character commands

- Status: Prepared; target compile/upload and physical/USB behavior pending.
- Verify/compile and upload results:
- h/1/0/b command replies and LED observations:
- All four line-ending choices:
- Unknown-byte tests and unchanged-state checks:
- 10, 01, 1b0 — predictions, replies, final state:
- Command sent during blink — approximate observed responsiveness:
- 3000 ms off-wait test — handler-duration calculation and restoration:
- Toggle extension and updated help/command table:
- Regression results and final configuration:
- Representative trace:
- Explanation of input polling versus blocking handling:

## Lesson 02 — Exercise 05: bounded line input

- Status: Prepared; target build and hardware/USB acceptance pending.
- Verify/compile and upload results:
- Command table and actual replies/LED states:
- Partial-line trace and experiment (or monitor limitation):
- LF/CR/CR+LF and empty-line observations:
- Exact matching tests (case and spaces):
- 15-byte versus 16-byte boundary inputs and replies:
- Oversized line with on suffix — rejection and subsequent recovery:
- Separate commands versus concatenated command:
- Partial-line reset behavior:
- Toggle extension and regression results:
- Explanation of capacity, null termination, and discard-until-boundary:

## Lesson 02 — Exercise 06: validated settings

- Status: Prepared; target build/upload and hardware/USB acceptance pending.
- Verify/compile and upload results:
- Valid setting trace and acknowledgements:
- Inclusive endpoints and just-outside values:
- Malformed token/command tests and unchanged-setting evidence:
- Leading zeros, long digit token, and line overflow:
- Independent setting changes and failed-update preservation:
- Reset/default restoration:
- Setting queued during blink:
- Previous-value acknowledgement extension and regression:
- 250/750 design — period, duty cycle, observations:
- Explanation of parse/validate/commit:

## Lesson 02 — Exercise 07: status queries

- Status: Prepared; target build/upload and USB/hardware acceptance pending.
- Verify/compile and upload results:
- Repeated-query trace and unchanged application fields:
- ON/OFF queries and physical observations:
- Accepted settings, blink, and completed counter:
- Rejected settings and preservation checks:
- Unknown/oversized/blank/CR+LF rejection counts:
- Query queued during blink:
- Two-blink/three-rejection sequence — prediction and captured final status:
- nominal_blink_ms extension and regression:
- Reset/defaults observations:
- Explanation of read-only snapshot and measurement limits:

## Lesson 02 — Exercise 08: error handling

- Status: Prepared; target build/upload and USB/hardware verification pending.
- Build/upload and monitor settings:
- Four-category trace and predicted/actual counts:
- Query isolation and accepted-command history:
- Oversized suffix and recovery:
- clear errors preservation check:
- Blank/CR/LF behavior:
- Timestamp/count extension and regression:
- Ten-command acceptance sequence and actual evidence:

## Lesson 02 — Exercise 09: protocol diagnosis

- Status: Prepared; target build/upload, learner fixes, and hardware evidence pending.
- Reference baseline and firmware revision:
- Case 1 — reproducer, expected/actual trace, hypothesis, fix, regression:
- Case 2 — reproducer, expected/actual trace, hypothesis, fix, regression:
- Case 3 — reproducer, expected/actual trace, hypothesis, fix, regression:
- Final fault selector or removed-fault implementation:
- Full regression sequence and evidence:

## Lesson 02 — Exercise 10: serial indicator capstone

- Status: Prepared; target build/upload, hardware acceptance, and extension pending.
- Final protocol and firmware revision:
- Baseline/defaults — pass/fail and evidence:
- Manual on/off/blink — pass/fail and evidence:
- Configured timing calculations and heartbeat observations:
- Burst timing calculations and observations:
- Stop during blocking work — approximate response evidence:
- Queries and counter meanings:
- Error categories and preservation:
- Oversized-line rejection/recovery:
- Error clearing isolation:
- Reset from each mode:
- Double-flash extension and regression:
- Final settings, representative traces, and limitations:

## Lesson 03 — Exercise 01: acceptance baseline

- Status: Prepared; physical run not performed or confirmed.
- Detailed evidence: exercises/lesson_03/01_acceptance_baseline/run-record.md
- Fixture and firmware identity:
- Predicted pre-reset and post-reset totals:
- First run outcome:
- Repeated run outcome:
- Learner-designed boundary/suffix cases:
- Deliberate default regression and restored-run result:
- Explanation of requirements, oracles, fixtures, and remaining limits:

## Lesson 03 — Exercises 02–10

All sources/guides are prepared. Physical results remain unconfirmed. Record detailed evidence beside each exercise in its evidence.md; generated tool reports go under build/.

- Exercise 02: exercises/lesson_03/02_boundary_cases/evidence.md — not run on hardware.
- Exercise 03: exercises/lesson_03/03_minimal_reproducer/evidence.md — not run on hardware.
- Exercise 04: exercises/lesson_03/04_pure_logic/evidence.md — not run on hardware.
- Exercise 05: exercises/lesson_03/05_host_checks/evidence.md — not run on hardware.
- Exercise 06: exercises/lesson_03/06_device_checks/evidence.md — not run on hardware.
- Exercise 07: exercises/lesson_03/07_responsiveness/evidence.md — not run on hardware.
- Exercise 08: exercises/lesson_03/08_fault_recovery/evidence.md — not run on hardware.
- Exercise 09: exercises/lesson_03/09_regression_suite/evidence.md — not run on hardware.
- Exercise 10: exercises/lesson_03/10_verification_capstone/evidence.md — not run on hardware.

## Device controller — application version 1.0.0

- Complete source prepared for built-in LED and USB.
- Host component tests and full Arduino CLI build passed; physical acceptance remains unconfirmed.
- Build memory: 42,104 bytes program, 4,080 bytes global/static RAM.
- Hardware evidence: projects/device_controller/HARDWARE_CHECKS.md
- Build/upload result and observations:
- Explanation of driver, controller, line reception, and cooperative scheduling:
