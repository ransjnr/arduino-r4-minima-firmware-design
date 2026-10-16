# Exercise 04 — Single-character commands

## Goal and command contract

Control the LED without editing and uploading a different program for each action. Receive bytes through USB Serial, dispatch commands, and produce predictable replies. Earlier serial examples transmitted information; this one receives it too.

| Byte | Action | Reply |
|---|---|---|
| h | Print help; leave LED unchanged | help |
| 1 | Hold LED on | on |
| 0 | Hold LED off | off |
| b | One 200 ms on / 800 ms off blink; finish off | blink_started, then blink_finished |
| CR or LF | Ignore line-ending byte | None |
| Anything else | Reject; leave LED unchanged | error with received byte number |

Commands are case-sensitive. Spaces are unknown commands. Each character is an independent command, not a word: sending `help` invokes h and then rejects e, l, and p. No line buffer is used yet.

## Run it

Open the sketch, select UNO R4 Minima, verify/upload, and open Serial Monitor at 115200. Select No line ending initially. Send h using the monitor's Send action or Enter. There is no unsolicited startup message: request help after connecting. Send 1 and 0 separately and compare replies with the LED. No extra wiring is required.

## Understand input handling

`Serial.available()` returns how many bytes are waiting in the receive buffer. It does not consume them. `Serial.read()` consumes the next byte, returning an int so it can also represent -1 when nothing is available. Store and check that result before dispatching.

`'1'` is the character representing digit one, not the integer 1. Double quotes such as `"ON"` represent text strings; single quotes such as `'b'` represent a character. `'\r'` is carriage return (CR) and `'\n'` is line feed (LF). A monitor may append either or both when you send text. Ignoring them prevents an extra error after a valid command.

The sketch processes at most one byte per loop call. With no byte available it immediately returns to the framework; there is no wait-for-input loop. The b handler still blocks on its delays, so not all parts of the application are responsive.

`on ? HIGH : LOW` is the conditional operator: choose HIGH when on is true, otherwise LOW. `ledIsOn` records the commanded state, not an independently measured physical state. The setLed helper updates the pin and state together.

## Trace a command sequence

For the received bytes `1b0`, the sketch holds the LED on, starts a blink, finishes off, then handles off. The final 0 may sit in the USB receive buffer while b runs. Bytes arrive and are processed in order; they are not guaranteed to arrive as one batch.

The handled_at_ms field is a board timestamp captured when preparing a response. It is not the time the user pressed Send or the byte reached USB. Subtracting timestamps for separately handled commands does not by itself measure end-to-end command latency.

## Substantial experiments

1. Upload and test h, 1, 0, and b individually. Record replies and LED behavior. Repeat 1 and 0 twice: explain why repeated set commands leave the same final state.
2. Test all four monitor line-ending choices: none, LF, CR, and CR+LF. The action should be identical without errors for the appended endings. Record actual behavior.
3. Send x, B, and a space. Confirm unknown bytes do not change the commanded LED state. Check the numerical byte values in the error records.
4. Send `10`, `01`, and `1b0` as short sequences. Predict reply order and final LED state before sending. Very fast on/off changes may be too brief to see; use replies as software evidence.
5. Send b and try sending 1 during its off wait. Explain why the on action cannot be handled until the blink handler returns. Observe delay from the computer as an approximate human/host observation, not a calibrated device latency measurement.
6. Temporarily set blinkOffMs to 3000. Send `b1` together and capture output. Compare blink_started and blink_finished timestamps: their difference measures the handler's waits plus reporting overhead. It does not measure USB transport latency. Restore 800 afterward.
7. Extend the protocol with t to toggle the LED: use setLed(!ledIsOn), then produce a toggle reply. Update help. Verify two toggles return to the original commanded state and that toggling works after b finishes.
8. Retest existing commands, CR/LF, and unknown bytes after your extension. Keep the final sketch, a short trace, and the updated command table in the notebook.

## Completion check and limitations

Explain available versus read, why read returns int, character versus numeric value, ignored line endings, and the difference between input polling and a blocking handler. Explain why a stream of bytes is not the same as complete text lines; the next exercise introduces bounded line assembly.

USB buffering is finite. Do not infer that arbitrarily long bursts of commands will be retained while the sketch waits. Use short sequences for these tests. If the monitor reconnects or a reset occurs, state and buffered input may change; document observed behavior rather than relying on it as persistent storage.

Status: source prepared; target compilation and physical/USB observations are pending.

## Official references

- https://docs.arduino.cc/language-reference/en/functions/communication/serial/available/
- https://docs.arduino.cc/language-reference/en/functions/communication/serial/read/
