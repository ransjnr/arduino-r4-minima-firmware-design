# Exercise 06 — Validated numeric settings

## Goal and protocol

Change blink durations through complete serial commands without uploading for each new value. Parse strictly, reject unsafe settings, and preserve the last valid configuration when a command fails.

| Command | Action |
|---|---|
| help | Show commands and numeric range |
| on / off | Hold the LED on / off |
| blink | Blink once using the current configured durations, ending off |
| set on N | Set ON duration to N milliseconds |
| set off N | Set OFF duration to N milliseconds |

N must contain only decimal digits and represent 50 through 5000 inclusive. Leading zeros are accepted. Signs, decimal points, unit suffixes, extra tokens, and whitespace inside/after the number are rejected. Commands are case-sensitive, with exactly one space between tokens. No trimming occurs. Each command needs LF, CR, or CR+LF.

Initial values are 200 ms on and 800 ms off. Settings live only in RAM and return to these defaults after reset. A setting changes future blink behavior, not the current LED state. A blink already running completes before queued settings are processed.

## VS Code workflow

This exercise is now the configured sketch in `.vscode/arduino.json`. Open it, **Arduino: Verify**, then **Arduino: Upload**. Close other monitors before uploading. Open Serial Monitor on the board's current port at 115200 and choose **Newline/LF**, not None. Send help first.

Try:

```text
set on 500
set off 1500
blink
```

Expected replies include setting_applied records and a blink_started record showing the current values. The nominal blink duration is 2000 ms plus logging/instruction overhead. No unsolicited startup output is required.

## Parse, validate, commit

The receive buffer remains bounded, now with 32 slots: 31 content bytes and a null terminator. The prior discard-until-boundary behavior remains intact.

Command dispatch uses `strncmp` to recognize a fixed prefix. `line + 7` points to the characters after `set on `; `line + 8` skips `set off `. Neither changes the input text. The next lesson on memory will develop pointers more fully; here they identify a position inside an existing terminated string.

The parser follows four steps:

1. Reject an empty numeric token.
2. Check every character is a digit. A prefix such as 100 in `100ms` must not be accepted.
3. Convert digits into a temporary candidate, checking the upper bound before multiplying by ten and adding the next digit.
4. Check the lower bound, then return the validated result.

`unsigned long& result` is a reference parameter: it can update the variable supplied by the caller. The parser only does that on success. `applySetting` then commits that temporary result to the actual setting. Failed input must never partially change the live configuration.

`const char*` is also used for returned error labels. A returned `nullptr` means no error; otherwise the returned fixed text is the reason. No heap-allocated String objects or permissive conversions are needed.

## Why check before arithmetic?

For max 5000, a candidate above 500 cannot accept another digit. At candidate 500, the next digit may only be 0. Testing these facts before multiplication keeps intermediate values bounded, even when the input contains many digits. The full token is checked for non-digits first, so a mixed token gets invalid_number rather than being treated as a valid numeric prefix.

## Error contract

| Input | Expected result |
|---|---|
| set on 50 / set on 5000 | Accepted endpoints |
| set on 49 / set on 5001 / set on 0 | out_of_range |
| set on -100 / set on 100ms / set on 1.5 | invalid_number |
| set on followed by a space and no digits | invalid_number |
| set on without the following space | unknown_command (prefix does not match) |
| set on 100 followed by a trailing space | invalid_number |
| set on 00050 | Accepted as 50 |
| Numeric token too long for the line buffer | line_rejected at its terminator |

Rejections do not change either timing or LED state. Each setting is a separate transaction: if set on succeeds and set off fails, the accepted ON setting remains; this is not an atomic two-setting command.

## Substantial experiments

1. Trace `set on 500` through receive, prefix recognition, conversion, validation, and assignment before uploading. Then test the example sequence above.
2. Test both inclusive endpoints and values just outside them. Set a known good value before each rejection; issue blink afterward and inspect its reported timings to prove preservation.
3. Test signs, suffixes, decimals, empty numeric input, extra spaces, and extra tokens. Distinguish malformed commands, malformed numbers, out-of-range numbers, and oversized lines.
4. Test leading zeros and a long all-digit token within the 31-byte content limit. Explain why conversion remains bounded even when the token could exceed the integer type's capacity.
5. Set only the ON duration. Confirm OFF is unchanged. Reject an OFF setting, then confirm ON still retains its accepted value.
6. Reset after successful settings. Reconnect if needed and request blink. Confirm defaults return; distinguish runtime configuration from nonvolatile storage.
7. Send blink, then a new setting while it is running. Observe that the setting is handled afterward. Explain why no input-parser improvement removes the handler's blocking waits.
8. Extend acknowledgements to include previous_ms and value_ms. Capture the old value before assignment. Keep validation unchanged and retest successful and failed updates.
9. Design a 250 ms ON / 750 ms OFF blink using commands only. Calculate period and duty cycle, confirm the reported settings, and record observations.

## Completion check

Explain digit characters versus numeric values (`text[i] - '0'`), exact prefix matching, references, temporary candidates, pre-arithmetic bounds checks, and commit-on-success. Explain why naive numeric conversion that accepts a prefix is insufficient for this protocol.

Status: source prepared; actual target build/upload and hardware/USB acceptance remain pending.
