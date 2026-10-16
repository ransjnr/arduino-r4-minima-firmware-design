# Lesson 03 — Systematic firmware testing and diagnosis

## Phase goal

Design repeatable acceptance cases, test boundaries, isolate logic, automate checks, and diagnose failures. Keep host tests separate from board builds, USB integration, and physical observations. No new hardware is required. Nonblocking scheduling remains Lesson 04.

## Exercise index

All ten exercises have source and instructions. Exercises 04–10 include their own copy of a small pure parser header so each sketch remains self-contained; the host suite checks all copies. Only Exercise 03 contains an intentional behavior defect. Exercise 08 has an explicitly disabled boot-fault selector.

| Exercise | Task | Guide |
|---|---|---|
| 01 | Versioned reset-based acceptance baseline | [Guide](../exercises/lesson_03/01_acceptance_baseline/README.md) |
| 02 | Boundary/equivalence matrix and preservation assertions | [Guide](../exercises/lesson_03/02_boundary_cases/README.md) |
| 03 | Minimize a seeded failure and retain a reproducer | [Guide](../exercises/lesson_03/03_minimal_reproducer/README.md) |
| 04 | Extract a pure parser with an explicit contract | [Guide](../exercises/lesson_03/04_pure_logic/README.md) |
| 05 | Run standalone C++ parser checks | [Guide](../exercises/lesson_03/05_host_checks/README.md) |
| 06 | Bounded Python runner against a connected board | [Guide](../exercises/lesson_03/06_device_checks/README.md) |
| 07 | Blocking responsiveness and measurement limits | [Guide](../exercises/lesson_03/07_responsiveness/README.md) |
| 08 | Invalid configuration injection and controlled recovery | [Guide](../exercises/lesson_03/08_fault_recovery/README.md) |
| 09 | Repeatable host/optional device regression workflow | [Guide](../exercises/lesson_03/09_regression_suite/README.md) |
| 10 | Verify a toggle feature against a written change request | [Guide](../exercises/lesson_03/10_verification_capstone/README.md) |

## Workflow

Exercise 01 remains selected; proceed in order. To select another exercise, update the sketch field in `.vscode/arduino.json` to its `.ino` path relative to the workspace. Use Arduino: Verify and Arduino: Upload. The guides specify each firmware identity. Moving forward does not mark earlier physical tests passed.

Serial Monitor: board's current port, 115200, Newline/LF. Close it before uploading or running a device test tool. Python tools require Python 3.9+; host C++ checks require g++. Device tools additionally require pyserial and a connected board. They never upload automatically. Physical reset differs from the test reset command introduced in Exercise 06.

Generated test reports go under build/ and stay outside version control; retain selected traces and conclusions in each evidence file. Each guide has a substantial implementation or test-design extension rather than treating prepared reference code as completed learning.

## Verification and tools

See [author checks and their limits](../exercises/lesson_03/author-checks.md). Run offline harness checks with `python3 exercises/lesson_03/tools/test_tooling.py`; they test the test tool itself and do not open USB.
