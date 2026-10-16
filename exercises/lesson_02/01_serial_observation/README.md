# Exercise 01 — Observe state through Serial Monitor

## Goal and specification

Run an LED heartbeat and report its commanded output state and completed-cycle count to the computer. Learn to relate a text trace to hardware actions. Use only the Minima and its USB data cable.

The LED is on for 200 ms and off for 800 ms per cycle. The sketch reports once after each output transition when a monitor is connected. Each complete cycle increments the count after both intervals finish. Serial output adds overhead, so the period is nominally one second, not an exact timer.

## Open and run

1. Open `01_serial_observation.ino` in Arduino IDE; select UNO R4 Minima and its port.
2. Verify and upload.
3. Open **Tools → Serial Monitor** and select **115200 baud**.
4. Observe at least five cycles and compare text with the LED. Re-select the port if it changes after upload/reset.
5. Reset with the monitor open to try to capture the first cycle. USB can disconnect and reconnect during reset; the BOOT line may still be missed. Later lines continue reporting, so seeing a BOOT line is not required to establish that the program runs.

`Serial` on this board is the USB connection to your computer. This is different from connecting external devices to hardware UART pins. We will study UART separately later. Use the matching 115200 setting as our convention; native USB serial does not behave exactly like a physical UART baud clock.

## Understand each new part

`Serial.begin(115200)` initializes the serial interface with the requested rate setting. `Serial.print` outputs human-readable text or numbers without ending the line. `Serial.println` outputs its value and then a line ending. Several print calls can build one line:

```cpp
Serial.print("completed_cycles=");
Serial.print(completedCycles);
Serial.print(" led=");
Serial.println(ledState);
```

Text inside double quotes is a string literal. `const char* ledState` lets this function receive a pointer to read-only characters, such as the literal `"ON"`. For now use it as a text parameter; pointers and memory will receive their own treatment later. This avoids introducing dynamic text allocation for simple fixed labels.

`if (Serial)` checks whether the USB serial connection is ready before reporting. The LED continues without an open monitor. We intentionally avoid `while (!Serial) { }`, which could keep the application waiting indefinitely for a computer connection. The optional BOOT line is attempted only once in setup; repeated state reports let you join the trace later.

## Trace the counter correctly

If the monitor is ready from the start, the event sequence begins:

```text
BOOT lesson=02 exercise=01
completed_cycles=0 led=ON
completed_cycles=0 led=OFF
completed_cycles=1 led=ON
completed_cycles=1 led=OFF
completed_cycles=2 led=ON
```

Both reports for the first cycle show zero because that cycle has not completed yet. The increment happens after the OFF wait. Joining later can start at any count or at an OFF report. The printed LED state describes the command our program issued, not an independent measurement of light output or electrical voltage.

## Experiments

1. Trace two full cycles on paper before uploading. Record build/upload results and capture at least ten consecutive output lines.
2. Explain why consecutive ON and OFF lines share a count. Compare the report positions with the counter update.
3. Change timing to 500 ms on and 1500 ms off. Predict cycle duration and approximate reporting frequency, upload, and compare with baseline.
4. Close Serial Monitor for several cycles, then reopen it. Confirm the LED continues and the counter advances while no reports are received. Host-side monitor behavior can vary; record what you actually see.
5. Reset and compare the counter with its previous value. Explain initialization versus persistent storage. Note any lost startup output or USB reconnection.
6. Add `on_ms` and `off_ms` fields to every report using the existing configuration constants. Keep each report on one line. Predict a sample line before implementing.
7. Temporarily move the increment to the beginning of loop. Predict how the meaning of the counter changes, observe, then restore the original position. Explain why a field named completed_cycles should count only completed cycles.
8. Restore your chosen valid timing. Save five sample lines, actual observations, and explanations in the lab notebook.

## Completion check and troubleshooting

Explain print versus println, initialization versus reporting, the counter's meaning, and why logging is evidence about software actions rather than proof of physical behavior. State that reporting adds work and can disturb timing; later exercises examine this more closely.

If no output appears, check the selected board/port, that the upload succeeded, and that no other terminal owns the port. Wait for repeated reports rather than relying on the BOOT message. A working upload does not mean the monitor is necessarily attached to the correct port.

Status: source prepared; target compilation and USB/hardware behavior have not been confirmed.

## Official references

- https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/
- https://docs.arduino.cc/language-reference/en/functions/communication/serial/print/
- https://docs.arduino.cc/language-reference/en/functions/communication/serial/println/
- https://docs.arduino.cc/tutorials/uno-r4-minima/cheat-sheet/
