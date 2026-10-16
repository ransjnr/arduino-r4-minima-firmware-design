# Device controller — first firmware increment

Build one application incrementally instead of creating a separate sketch for every small change. The target is the UNO R4 Minima. This first increment needs only the built-in LED and USB cable.

## Structure

- device_controller.ino: application initialization, command dispatch, and cooperative main loop.
- src/StatusLed.h: public interface and private state for the indicator component.
- src/StatusLed.cpp: pin ownership, hardware output, and timed transitions.

The status component is a small driver built on the Arduino hardware API. We still use Arduino for startup, USB, clock, and GPIO access. Direct register programming and interrupt handling are future increments, introduced with the board's hardware documentation when needed.

## First run in VS Code

The workspace selects this sketch. Save files, use Arduino: Verify, then Arduino: Upload. Close competing monitors before uploading. Open the selected board port at 115200. Send h to request help; there is no startup greeting. This is a character protocol, so CR/LF are ignored and no newline is required by the firmware; your monitor may still require Enter to transmit.

| Character | Action |
|---|---|
| b | Start/restart blinking: 200 ms on, 800 ms off |
| s or 0 | Stop and command LED off |
| 1 | Stop blinking and hold LED on |
| ? | Report mode and current commanded state |
| h | Help |

## What makes it responsive

update checks whether enough time has elapsed, performs at most one transition, then returns. It never waits for the interval to finish. loop can check serial input on every pass, including while the LED is on. start resets the phase to ON; hold cancels automatic transitions.

Unsigned timestamp subtraction handles clock wrap for intervals shorter than the full 32-bit wrap period. When a loop is late, the driver transitions once and starts the next interval from the actual transition time. It does not replay missed transitions or promise a fixed absolute phase. USB output can still block, so removing delay is not a hard real-time guarantee.

## Work through the code

1. Read the header first. Identify the operations the application may request and the state it cannot modify directly.
2. Follow b from command dispatch to start, then through repeated calls to update. Trace the first transition at 200 ms and the next at 1000 ms, ignoring normal execution overhead.
3. Upload and send b. While blinking, send ? repeatedly, then s while the LED is on. Observe whether it stops without waiting for a complete cycle.
4. Compare this behavior with Lesson 02's blocking blink handler. Explain where execution returns to input handling in each design.
5. Change the private durations to 2000 ms on and 3000 ms off. Upload, then send s during the long ON phase. Confirm the five-second cycle does not imply a five-second command wait. Restore your chosen values afterward.
6. Add a restart command r that reuses start. Keep GPIO operations inside the driver. Document your final behavior and physical observations.

## Next project increments

Add a proper operating-mode state machine; define command and timing ownership; add an available button/input and debounce; introduce an interrupt with a narrowly scoped handler; connect an available peripheral and implement its driver; study GPIO registers and compare framework calls with the underlying hardware. Select actual wiring and electrical limits after identifying the components you have.

Build/upload and physical observations remain to be recorded. A code or host syntax check is not a complete device acceptance run.
