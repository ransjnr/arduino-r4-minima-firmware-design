# Exercise 02 — Boundary and equivalence cases

## Fixture

Board: UNO R4 Minima. Firmware identity: `lesson03_ex02_v1`. Sketch: `exercises/lesson_03/02_boundary_cases/02_boundary_cases.ino`. To build this exercise, update the `sketch` field in `.vscode/arduino.json` to that path. Keep the configured board and current port. Use Arduino: Verify and Arduino: Upload; close serial monitors before uploading. Serial Monitor uses 115200 and Newline/LF. Source preparation is not evidence of hardware execution.

## Goal

Replace a few arbitrary examples with justified input partitions and adjacent boundary values. Use the unchanged Lesson 02 protocol with a new firmware identity. The task is to design tests, not add settings or commands.

## Required case matrix

Create `case-matrix.md` with columns: ID, partition, precondition, exact input bytes/line ending, expected response, expected state, actual evidence. Include at least these classes:

- Digit-only values below range, at 50, inside range, at 5000, and above range. Include 0, 49, 50, 51, 4999, 5000, 5001.
- Empty numeric tokens, signs, decimal points, suffixes, leading/trailing spaces, multiple tokens, and leading zeros.
- Exact recognized words versus prefixes, suffixes, case differences, and blank lines.
- 30, 31, and 32 content bytes. The first two fit; an unknown word still fails dispatch. The third must be discarded as a line error.
- LF, CR, and CR+LF. Include repeated blank boundaries without double-counting.

## Implementation and experiments

1. Start from reset and request version/status/errors. Record actual fixture identity.
2. Before every rejection test, establish a known LED state and timing. Afterward query them to prove preservation; an error reply alone is insufficient.
3. Predict the exact rejection-category increments, including whether numeric parsing or command dispatch is reached. Test both ON and OFF settings rather than assuming symmetry.
4. For a maximum-length accepted command, construct `set on ` followed by enough leading zeros and `50` to total 31 bytes. Confirm it commits 50. Add one leading zero to produce 32 bytes; it must reject the line and preserve the prior value.
5. Test a rejected line with a valid `on` suffix while LED is off. Then send a fresh valid line to prove recovery.
6. Choose one representative per partition, explain what it covers, and identify cases that require distinct boundary tests. Reduction must not erase the 31/32 distinction.
7. Repeat your reduced suite from reset and compare with the full matrix. Keep evidence in evidence.md and the new matrix.

## Completion check

Explain why valid/invalid partitions do not prove every possible input, why boundary neighbors deserve separate tests, and why byte length differs from user-perceived text length. This protocol is printable ASCII only. No hardware result is pre-marked passed.

## Evidence

Record expected/actual results, failed/blocked cases, changes, and separate physical observations in [evidence.md](evidence.md). Do not prefill passes from source inspection or host-only results.
