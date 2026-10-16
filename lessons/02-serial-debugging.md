# Lesson 02 — Serial communication and debugging

## Phase goal

Observe internal program state, send commands, validate input, and diagnose behavior through a USB serial console. Build on Lesson 01's functions, variables, conditions, and patterns. No extra components are needed.

## Ten-exercise plan

Each exercise includes a substantial implementation task, experiments, and evidence recorded in the notebook. All ten exercises have source and guides. Exercise 08 is the next task; preparation is distinct from hardware completion. Hardware completion is recorded separately.

| Exercise | Build task | Completion evidence |
|---|---|---|
| 01 — Observe state | Report LED transitions and completed cycles using print/println. | Trace output, correlate it with hardware, and explain missed startup lines. |
| 02 — Structured diagnostics | Add readable event fields, startup configuration, and log levels. | Distinguish configuration, state, and events without excessive repetition. |
| 03 — Elapsed time | Add millis-based timestamps to logs and compare expected timing with observed intervals. | Account for delays and logging overhead; identify measurement limits. |
| 04 — Character commands | Read available bytes and implement help and LED commands. | Handle known/unknown commands and CR/LF explicitly; measure blocking latency. |
| 05 — Line input | Assemble commands into a fixed-size bounded buffer. | Test empty, partial, maximum-length, and overflowing lines. |
| 06 — Validated settings | Parse numeric duration commands with explicit range checks. | Reject malformed/out-of-range input without changing valid settings. |
| 07 — Status queries | Expose mode, timing, counters, and errors without changing state. | Verify responses match the running application. |
| 08 — Error handling | Count and report rejected commands with useful error messages. | Distinguish input errors from configuration faults; recover predictably. |
| 09 — Reproduce and diagnose | Use captured logs and command sequences to investigate seeded defects. | Record reproducible steps, hypotheses, corrections, and regression evidence. |
| 10 — Serial-controlled indicator | Integrate commands, configuration, status, and bounded input handling. | Execute a documented acceptance sequence and explain remaining blocking limitations. |

Changing to nonblocking scheduling is a later phase. This lesson first makes the current design's behavior and limitations visible. millis timestamps here are for observation, not yet for replacing delay.

## Current exercise

Read [Exercise 08: error handling](../exercises/lesson_02/08_error_handling/README.md) and open its sketch. Keep earlier lesson sketches intact.

Lesson 01 remains available for reference. Moving to this lesson does not mark its unreported hardware checks as passed.

## Exercise guides

- [01 — Observe state](../exercises/lesson_02/01_serial_observation/README.md)
- [02 — Structured diagnostics](../exercises/lesson_02/02_structured_diagnostics/README.md)

- [03 — Elapsed time](../exercises/lesson_02/03_elapsed_time/README.md)
- [04 — Character commands](../exercises/lesson_02/04_character_commands/README.md)
- [05 — Bounded line input](../exercises/lesson_02/05_line_input/README.md)
- [06 — Validated settings](../exercises/lesson_02/06_validated_settings/README.md)
- [07 — Status queries](../exercises/lesson_02/07_status_queries/README.md)
- [08 — Error handling](../exercises/lesson_02/08_error_handling/README.md)
- [09 — Protocol diagnosis](../exercises/lesson_02/09_protocol_diagnosis/README.md)
- [10 — Serial indicator capstone](../exercises/lesson_02/10_serial_indicator/README.md)
