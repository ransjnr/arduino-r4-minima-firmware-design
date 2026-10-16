# Acceptance cases — reference run

## Fixture and oracle

UNO R4 Minima, firmware `lesson03_ex01_v1`, 115200 monitor, LF line endings. Physically reset before the run. The following cases share state and must execute in order. Send commands individually and wait for their replies. `<LF>` below describes the selected line ending; do not type the characters `<LF>`.

| ID | Input/action | Expected software result | Physical check / pass rule |
|---|---|---|---|
| A01 | version, then status, then errors | Version matches; status manual/OFF, on=200, off=800, completed=0, rejected=0; all error categories zero, last=none | LED off; ignore exact timestamp |
| A02 | on, status, status | Both statuses ON with defaults and zero counters | LED stays on through both queries |
| A03 | set on 400; set off 600; status | Two setting acknowledgements; timings 400/600, LED ON, counters zero | Settings alone do not turn LED off |
| A04 | set on bad; set off 49; unknown; errors; status | Three rejections: numeric=1, range=1, unknown=1, line=0, total=3, last=unknown_command; timings stay 400/600, LED ON, completed=0 | LED remains on; rejected changes preserve configuration |
| A05 | blink; status | started with 400/600; finished OFF; completed=1, rejected=3 | One visible pulse ending off; no exact timing claim from eyesight |
| A06 | clear errors; errors; status | All error categories and total=0, last=none; completed remains 1, timings 400/600, LED OFF | Clearing diagnostics does not reset application state |
| A07 | Send exactly 31 x characters plus LF; errors | unknown_command; unknown=1, total=1, line=0 | The maximum-sized line reaches dispatch, not length rejection |
| A08 | Send exactly 32 x characters plus LF; errors | line_rejected; unknown=1, line=1, total=2, last=line_rejected | No unintended LED change |
| A09 | on; status; off; status | First status ON, second OFF; timings 400/600, completed=1, rejected=2 | Proves parser recovery after rejected line |
| A10 | Physically reset; reconnect if necessary; version; status; errors | Defaults and zero statistics restored; version still matches | LED off; reset differs from clear errors |

For A07: `xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx` (31 x characters).

For A08: `xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx` (32 x characters).

Do not include Markdown backticks in sent input. Timestamps may increase or restart after reset; compare their meaning rather than exact values. A07/A08 copy lengths are part of the test setup and must be checked.

Before A10, the final expected application state is manual/OFF, timings 400/600, completed_blinks=1, rejected_commands=2; unknown=1, line=1, numeric=range=0. After A10 all runtime defaults return.

## Evidence rules

Mark pass only after checking all assertions for that case. Mark fail for mismatches with a captured trace; mark blocked if the port/build prevents execution; mark not run otherwise. Keep software output and LED observation separate. A test that passes once does not establish behavior under all possible inputs.
