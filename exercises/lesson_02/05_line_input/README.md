# Exercise 05 — Assemble bounded command lines

## Goal and contract

Receive complete words instead of executing each character. Build one line incrementally without waiting for missing bytes. Protect a fixed-size buffer and recover cleanly from rejected input.

Commands are exactly `help`, `on`, `off`, and `blink`, terminated by LF, CR, or CR+LF. Matching is case-sensitive; spaces are significant and are not trimmed. Empty lines are ignored. Unknown complete commands produce one error and do not change LED state. Blink uses 200 ms on and 800 ms off, ending off.

The buffer has 16 slots: at most 15 printable ASCII content bytes plus a null terminator. Longer lines or lines containing nonprintable/non-ASCII bytes are discarded in full until the next line ending. A rejected line produces one error at its end. This exercise deliberately uses a simple ASCII protocol, not arbitrary Unicode input. Backspace is a rejected control byte, not editing support.

## VS Code workflow

The workspace now selects this sketch for upload. Open it, run **Arduino: Verify**, then **Arduino: Upload**. Use regular Upload, not Upload Using Programmer. Open Serial Monitor at 115200 and select Newline/LF as the line ending. Send `help` first. Close the monitor before uploading again.

With No line ending, text remains partial and must not execute. A serial Send action is not inherently a device-side command boundary: the appended CR/LF byte creates the boundary.

## New concepts

`char lineBuffer[16]` reserves 16 character slots with indices 0 through 15. `lineLength` tracks how many content bytes have been stored. Before appending, we require `lineLength < lineCapacity - 1`, reserving space for `\0`, the C-string terminator.

`'\0'` is a zero byte marking the end of a C string. It differs from the printable digit `'0'`. We append it only when a valid nonempty line is complete. `strcmp(a, b)` compares C strings by content and returns zero when they match; pointer equality would not compare their text.

`static_cast<char>(incoming)` explicitly converts a validated printable input value to a character. Serial.read returns int so it can represent -1; we validate that before storing anything.

The receive state is lineBuffer, lineLength, discardingLine, and previousWasCR. These persist across loop calls, allowing one command to arrive in pieces. Each call handles at most one available byte. There is no timeout; an unfinished line stays pending until terminated or reset.

## Why discard until a boundary?

If an oversized line were merely truncated or the buffer immediately cleared, a later suffix such as `on` might accidentally become an executable command. Once invalid, we ignore all remaining content until a line ending. The boundary reports the rejection and resets receive state so the next complete line can be processed normally.

CR finishes a line immediately. A directly following LF is swallowed so CR+LF is one boundary. A separate LF finishes its own line. Since empty lines are ignored, repeated boundaries do not create command actions.

## Experiments and acceptance cases

1. Read the code in order: consumeByte → finishLine → handleLine. Trace `on` followed by LF. Explain why no action occurs after just o or n. Verify/upload and test all four commands.
2. Select No line ending and send `o`, then `n`. Confirm no action yet. Select LF and send an empty entry if your monitor permits it, or a separate terminating newline using a serial sender. Confirm `on` executes once. If your monitor cannot send an empty entry, use the test below instead; do not assume the experiment ran.
3. With LF selected, send `on` normally. Repeat with CR and CR+LF. Confirm one action/reply for each submitted command. Send an empty line and confirm no action.
4. Send `ON`, `on `, and `unknown` as complete lines. Verify each is rejected as an unknown command without changing the LED. Spaces are allowed content, so they cause a mismatch rather than a line-format rejection.
5. Send `123456789012345` (15 content bytes) plus newline. It should fit and reach command matching, producing unknown_command. Send `1234567890123456` (16 bytes) plus newline: it should produce line_rejected. Explain the difference.
6. Hold the LED off, then send `1234567890123456on` plus newline. Confirm the suffix does not turn the LED on. Send a fresh `on` line afterward and confirm recovery.
7. Send two separate lines `on`, then `off`, and record ordering. Send `onoff` as one line and explain why it is a single unknown command rather than two valid commands.
8. Start a partial command with No line ending, then reset the board. After the port reconnects, send a complete command. Explain how reset affects receive state. Monitor reconnect alone is not defined as clearing a partial line in this sketch.
9. Extend the protocol with `toggle`, update help, and retest exact matching, boundary lengths, oversized suffix rejection, and recovery. Use setLed to keep software state and output commands together.
10. Keep your final implementation and record inputs, replies, and LED observations in the notebook.

## Completion check and limits

Explain capacity versus content length, why the terminator needs a slot, how partial lines survive loop calls, and why discarded content cannot become a new command. Explain why read polling is incremental but the blink handler still blocks for its complete pattern. USB receive buffering is finite; these tests do not establish support for unlimited incoming data during a blink.

The next exercise will parse and validate numbers inside complete lines. Do not add numeric settings or timeouts here yet.

Status: source prepared; target compilation and actual USB/hardware acceptance remain pending.
