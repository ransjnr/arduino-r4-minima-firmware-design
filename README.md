# Firmware Design

A practical firmware development repository built around the **Arduino UNO R4 Minima**. It contains progressive exercises and a complete device-controller application using the board's built-in LED and USB serial connection.

The current project develops application structure, hardware drivers, state machines, nonblocking timing, bounded command input, configuration validation, and diagnostics. The MXCHIP IoT DevKit is available for future work; it is not a supported target of the current controller.

## Current project: Device Controller

Firmware runs on the microcontroller: it receives commands, makes decisions, and controls hardware. This application implements manual LED control, cancellable one-shot blinking, and repeating status patterns while continuing to service serial input.

**Start with the [project guide](projects/device_controller/README.md) and [application sketch](projects/device_controller/device_controller.ino).** The VS Code workspace currently selects this sketch for build and upload.

### Features

- Manual ON/OFF control and toggle.
- One-shot blink, repeating heartbeat, three-flash burst, and five-flash alert.
- Nonblocking elapsed-time checks with 32-bit timestamp rollover handling.
- Runtime ON/OFF settings, validated within 50–5000 ms.
- Fixed-size command buffer: 63 printable ASCII content bytes per line.
- Recovery from oversized or invalid lines without executing their suffixes.
- Status snapshots, completion counters, and categorized input errors.
- C++ component tests using simulated GPIO, time, and serial input.

### Architecture

```text
USB serial bytes → LineReceiver → Controller → StatusLed → GPIO / built-in LED
                                      │
                                      ├── Duration validation
                                      └── Status and error responses → USB serial
```

| Component | Responsibility |
|---|---|
| `device_controller.ino` | Construct the application and delegate startup/main-loop work |
| `Config.h` | Firmware identity, default timings, bounds, and receive budget |
| `Controller.h/.cpp` | Commands, operating modes, settings, counters, and diagnostics |
| `StatusLed.h/.cpp` | Own the output pin and advance timed pattern segments |
| `LineReceiver.h/.cpp` | Assemble bounded lines and recover at line endings |
| `Duration.h` | Validate decimal input before committing a setting |

The controller processes at most 16 available input bytes per loop pass, then services the LED driver. The application contains no `delay()` calls or wait-for-input loops. Serial output can still block, so this cooperative design does not provide a hard real-time response guarantee.

## Hardware and development setup

### Required

- Arduino UNO R4 Minima and a USB-C data cable.
- VS Code with **Arduino Community Edition**, extension ID `vscode-arduino.vscode-arduino-community`.
- Arduino CLI, either bundled with the extension or installed separately.
- **Arduino UNO R4 Boards** installed through the extension's Board Manager.

No external circuit or peripheral is needed for the current application. It uses the Arduino framework for startup, GPIO, timekeeping, and USB communication; it does not yet implement direct register access or application interrupt handlers.

### Build and upload in VS Code

1. Open this repository folder in VS Code.
2. Enable Arduino CLI in the Arduino extension settings. To use its bundled CLI, leave custom CLI paths empty.
3. Run **Arduino: Board Manager** and install **Arduino UNO R4 Boards** if needed.
4. Select **Arduino UNO R4 Minima** and the connected board's actual serial port.
5. Check `.vscode/arduino.json`: the sketch should be `projects/device_controller/device_controller.ino`, with board ID `arduino:renesas_uno:minima`.
6. Save files, close serial monitors, run **Arduino: Verify**, then **Arduino: Upload**.
7. Open Serial Monitor at **115200** and select **Newline/LF**. Send `version`, then `help`.

The port may change after reconnecting/resetting the board; `/dev/ttyACM0` is an example, not a fixed requirement. Uploading and monitoring must not compete for the same port.

The application does not require an open monitor to start, and it does not emit a mandatory startup greeting. If Linux reports `LIBUSB_ERROR_ACCESS`, see Arduino's [udev permissions instructions](https://support.arduino.cc/hc/en-us/articles/9005041052444-Fix-udev-rules-on-Linux).

## Command interface

Send each command as a complete line. LF, CR, and CR+LF are accepted; empty lines are ignored. Matching is case-sensitive, with exact spacing and no trimming. Single-character aliases also require a line ending.

| Command | Action |
|---|---|
| `help` / `h` | Show commands and syntax |
| `version` | Report firmware identity |
| `status` / `?` | Query mode, commanded LED state, timings, and counters |
| `on` / `1` | Cancel patterns and hold LED on |
| `off` / `0` / `stop` / `s` | Cancel patterns and hold LED off |
| `toggle` | Cancel patterns and invert the commanded state |
| `blink` | Start/restart one configured ON/OFF cycle |
| `mode manual` | Cancel patterns and hold LED off |
| `mode heartbeat` / `b` | Repeat the configured ON/OFF cycle |
| `mode burst` | Repeat three short flashes; nominal 1000 ms cycle |
| `mode alert` | Repeat five short flashes; nominal 2000 ms cycle |
| `set on N` / `set off N` | Set a duration in milliseconds, inclusive range 50–5000 |
| `errors` | Query rejection categories and last error |
| `clear errors` | Clear rejection history only |

Try this sequence, sending commands separately:

```text
version
set on 2000
set off 3000
mode heartbeat
status
stop
status
```

Observe whether `stop` ends the ON phase without waiting for the five-second cycle. Complete command reception is required before an action can execute.

A successful timing edit restarts heartbeat or one-shot mode in its ON phase. Manual output and fixed burst/alert patterns remain unchanged. Failed edits preserve settings. Reset restores manual/OFF, default 200/800 ms timings, and zero counters; settings are not saved to nonvolatile storage.

See the [project guide](projects/device_controller/README.md) for timing tables, counter semantics, command details, and CLI build/upload examples.

## Verification

Run host component tests from the repository root with Python 3 and g++ installed:

```bash
python3 projects/device_controller/tests/run_tests.py
```

These compile production components against simulated hardware. Coverage includes numeric limits, receive-buffer recovery, timestamp rollover, pattern timing, cancellation, setting changes, and command state transitions.

| Verification layer | Recorded status |
|---|---|
| Host component tests | Passed |
| ARM compiler syntax checks | Passed |
| Full Arduino CLI build for UNO R4 Minima | Passed on 2026-10-17 |
| Program storage | 42,104 / 262,144 bytes |
| Global/static RAM | 4,080 / 32,768 bytes |
| Current project upload and physical acceptance | Pending confirmation |

Build figures apply to the recorded firmware build, not every future revision. Static RAM figures exclude additional runtime stack requirements. Host checks do not prove physical GPIO, USB behavior, or measured response latency. Record board observations in the [hardware checklist](projects/device_controller/HARDWARE_CHECKS.md).

## Repository layout

```text
firmware-design/
├── projects/device_controller/
│   ├── device_controller.ino
│   ├── src/                    # Application, driver, receiver, parser, config
│   ├── tests/                  # Host tests and simulated hardware
│   ├── README.md
│   └── HARDWARE_CHECKS.md
├── lessons/                    # Learning guides
├── exercises/
│   ├── lesson_01/              # LED control and C++ foundations
│   ├── lesson_02/              # Serial commands and diagnostics
│   └── lesson_03/              # Testing, diagnosis, and reusable tools
├── notes/lab-notebook.md
└── .vscode/                    # Local board/sketch/editor configuration
```

Generated build outputs and local Python environments are ignored by Git. Some VS Code IntelliSense paths reflect the local toolchain installation and may need regeneration or adjustment on another computer.

## Learning material

| Lesson | Focus | Material |
|---|---|---|
| 01 | GPIO output, timing, C++ foundations | [Ten exercises](lessons/01-blink.md) |
| 02 | Serial communication, validated commands, diagnostics | [Ten exercises](lessons/02-serial-debugging.md) |
| 03 | Acceptance cases, boundaries, host/device tests, fault diagnosis | [Ten exercises and tools](lessons/03-testing-diagnosis.md) |
| 04 | Responsive firmware architecture and state machines | [Device controller project](lessons/04-firmware-project.md) |

Prepared exercises are reference material; their existence does not mean all learner extensions or hardware checks have been completed. Earlier lesson tools expect their own firmware versions and protocols and should not be run unchanged against the device controller.

Record predictions, observations, explanations, and unresolved issues in the [lab notebook](notes/lab-notebook.md). Future project increments can add actual inputs, peripheral drivers, hardware timers, interrupts, and register-level investigation once the hardware requirements are defined.

## References

- [UNO R4 Minima documentation](https://docs.arduino.cc/hardware/uno-r4-minima/)
- [Arduino CLI documentation](https://docs.arduino.cc/arduino-cli/)
- [Arduino sketch build process](https://docs.arduino.cc/arduino-cli/sketch-build-process/)
- [Arduino Community Edition for VS Code](https://marketplace.visualstudio.com/items?itemName=vscode-arduino.vscode-arduino-community)
