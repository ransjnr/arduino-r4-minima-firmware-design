# Exercise 05 — Run standalone C++ parser checks

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex05_v1`. Sketch: `exercises/lesson_03/05_host_checks/05_host_checks.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal

Compile and execute logic tests on your computer without a connected board. `parser_tests.cpp` is an actual test program with an independent interval oracle, not a copy of the parsing algorithm.

From the project root in VS Code's terminal:

```bash
python3 exercises/lesson_03/tools/run_host_checks.py
```

Python 3.9+ and g++ must be installed. The script builds temporary test binaries, checks each Lesson 03 parser copy, and writes `build/lesson03/host.json`. Exit 0 means these host checks passed; 1 means a failure; 2 means blocked tooling. It does not upload, open USB, or establish Arduino target compatibility.

## Coverage and tasks

1. Read the tests before running them. Predict accepted versus rejected values at both endpoints and why sentinel output 777 is used on failures.
2. Run the suite, inspect the JSON, and identify each tested header. More parser copies are tested so later independent sketches cannot silently drift.
3. Explain the exhaustive numeric check from 0 through 10000, malformed tokens, huge tokens, and leading-zero cases. Long pure-parser inputs exceed the device line limit deliberately: the parser's contract and receiver's contract are different layers.
4. Add tests for embedded whitespace, uppercase suffixes, and successful parsing when the previous output sentinel equals a valid setting. Use explicit expected outcomes, not logic copied from the parser.
5. In this exercise's header only, change the lower bound to 51. Predict which test fails and run the suite. Restore 50 and rerun; retain the failure evidence rather than leaving the mutation active.
6. Try a failure-path mutation that writes result before returning an error, then confirm the preservation assertion catches it. Restore and rerun.
7. Extend the host tool to summarize test cases if desired, but do not count a missing compiler as pass or conflate tested copies with hardware coverage.

## Completion check

Explain unit test versus integration test, independent oracle versus implementation mirroring, sentinel assertions, deterministic results, mutation testing, and why a successful host compile is not a Minima firmware build.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
