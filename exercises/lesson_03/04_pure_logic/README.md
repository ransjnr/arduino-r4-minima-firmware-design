# Exercise 04 — Isolate pure numeric parsing

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex04_v1`. Sketch: `exercises/lesson_03/04_pure_logic/04_pure_logic.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal and implementation

Separate numeric conversion from serial output, LED operations, and settings mutation. The supplied reference extracts a header-only parser into `logic/duration.h`; the sketch wrapper delegates to it. A subfolder avoids the extension's header-discovery limitation. Each later sketch has a self-contained copy of this header.

Contract: input points to a null-terminated string. Success returns nullptr and writes a value in 50..5000 inclusive. Failure returns invalid_number or out_of_range and leaves the output unchanged. Null pointers and unterminated memory are outside this function's contract; the bounded receiver constructs a valid terminated string.

## Work and evidence

1. Compare the old embedded parser with the extracted function. List dependencies removed or avoided: no Serial, no pin writes, no delay, no live timing target.
2. Trace `set on 500` through line reception, pure parsing into a temporary, then commit. Explain why “pure” here means no observable I/O/global mutation; the explicit result reference is an output, not hidden state.
3. Verify/upload and run valid, malformed, boundary, and failed-update preservation cases before changing anything else. Compare behavior with Exercise 02's matrix.
4. In your own working copy, add a small `isDecimalDigit` helper inside the namespace and use it in the lexical pass. Preserve the public contract. Do not introduce logging into the parser.
5. Run the same board cases after refactoring. Verify error precedence on a huge digit prefix followed by x: malformed token should return invalid_number because the lexical pass examines the whole token first.
6. Document the include path and why header-only functions are inline. Explain how separate responsibilities make the next exercise's host checks possible.

## Completion check

Explain deterministic inputs/outputs, explicit contracts, failure preservation, header inclusion, namespaces, inline linkage, and why a host parser test will not prove UART/USB or LED behavior. Record source changes and before/after evidence.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
