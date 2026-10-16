# Device controller firmware

Complete application source for the Arduino UNO R4 Minima's built-in LED and USB serial connection. The firmware provides manual output, a cancellable one-shot blink, three repeating status patterns, runtime timing configuration, diagnostics, and bounded line reception. External peripherals are outside this hardware configuration.

## Build and upload in VS Code

1. Open the firmware-design workspace. `.vscode/arduino.json` selects `projects/device_controller/device_controller.ino` and `arduino:renesas_uno:minima`.
2. Ensure Arduino UNO R4 Boards is installed through Arduino: Board Manager.
3. Save files, close serial monitors, run Arduino: Verify, then Arduino: Upload. Select the board's current port if it changes.
4. Open Serial Monitor at **115200** and choose **Newline/LF**. Send `version`, then `help`.

The application emits no mandatory startup greeting and never waits for a monitor connection. A physical reset restores manual/OFF, default 200/800 ms timings, and zero counters. No settings are stored in nonvolatile memory.

Optional command-line build from the repository root:

```bash
arduino-cli compile --fqbn arduino:renesas_uno:minima --build-path "$PWD/build/device_controller" "$PWD/projects/device_controller"
```

Upload only after a successful build, replacing the example port as needed:

```bash
arduino-cli upload --fqbn arduino:renesas_uno:minima --port /dev/ttyACM0 --input-dir "$PWD/build/device_controller" "$PWD/projects/device_controller"
```

## Command protocol

Every command, including single-character aliases, must end with LF, CR, or CR+LF. Empty lines are ignored. Commands are exact, case-sensitive printable ASCII with no whitespace trimming. Maximum content length is 63 bytes; the 64th buffer slot holds the null terminator. Oversized or nonprintable lines are discarded through their terminator, producing one error. Their suffix cannot become a command.

| Command | Effect |
|---|---|
| help / h | Print supported commands and syntax |
| version | Print firmware identity: device-controller-1.0.0 |
| status / ? | Report timestamp, mode, commanded LED state, timings and counters |
| errors | Report rejection totals/categories and last reason |
| clear errors | Clear rejection history only |
| on / 1 | Cancel automatic output; hold LED on |
| off / 0 / stop / s / mode manual | Cancel automatic output; hold LED off |
| toggle | Cancel automatic output; invert current commanded state |
| blink | Start/restart one configured ON/OFF cycle, ending off |
| mode heartbeat / b | Start/restart repeating configured ON/OFF cycles |
| mode burst | Repeat the fixed three-flash pattern |
| mode alert | Repeat the fixed five-flash pattern |
| set on N | Validate and set configured ON duration |
| set off N | Validate and set configured OFF duration |

N is a decimal integer from **50 to 5000 milliseconds inclusive**. Leading zeros are accepted. Signs, suffixes, decimals, trailing spaces and extra tokens are rejected. Failed settings preserve configuration and mode. Input categories are line, command, and setting; last-error strings are fixed labels, not pointers into a reused input buffer. These diagnostics do not measure hardware health.

Successful timing changes restart heartbeat or one-shot mode immediately in its ON phase. Manual output remains unchanged, while burst/alert keep their fixed timings. This explicit policy applies even when the new timing equals the old timing. Separate setting commands are separate commits, not one atomic two-setting transaction.

## Timing and cancellation

| Mode | Definition | Nominal cycle |
|---|---|---|
| manual | Hold requested state | No automatic transitions |
| once | Configured ON, configured OFF, finish in manual/OFF | Default 1000 ms |
| heartbeat | Repeat configured ON/OFF | Default 1000 ms |
| burst | Three 100 ms ON / 100 ms OFF pulses, plus 400 ms extra OFF | 1000 ms |
| alert | Five 100 ms ON / 100 ms OFF pulses, plus 1000 ms extra OFF | 2000 ms |

No application `delay()` or wait-for-input loop is used. On every loop pass, the controller consumes at most 16 available bytes and services one timed output transition. A partial command does not suspend the pattern. `stop` is executed after its complete line is assembled; it cancels either a repeating pattern or a one-shot immediately at handling time.

The driver uses unsigned 32-bit elapsed-time subtraction, valid across timestamp wrap for intervals shorter than a full wrap period. Late calls advance one real edge and restart that interval from the actual service time. They do not replay missed flashes or maintain absolute clock phase.

Serial output itself can block depending on USB connection and buffering. Long replies or high input traffic can delay service; this cooperative design makes no hard real-time latency guarantee. stop is a normal protocol command, not an emergency hardware interlock.

## State and counters

- completed_blinks increments only after an entire one-shot ON/OFF cycle finishes. Cancellation or restart before completion does not count.
- completed_patterns increments on each complete repeating cycle across mode changes.
- rejected_lines increments once for each rejected nonempty/invalid complete line. Category counters identify line-format, unknown-command, and numeric-setting errors.
- Queries do not change application state, but the timed driver may advance after a query in the same loop pass. Consecutive status snapshots during operation can therefore differ.
- Counters are unsigned RAM values; they wrap eventually and reset with the board. Error clearing preserves timings and action counts.
- The LED field is software's commanded state, not independent optical or electrical feedback.

## Source responsibilities

| File | Responsibility |
|---|---|
| device_controller.ino | Construct application; delegate setup and loop |
| src/Config.h | Firmware identity, numeric limits, buffer and receive budget |
| src/Controller.h/.cpp | Operating modes, command dispatch, counters and diagnostics |
| src/StatusLed.h/.cpp | Own output pin; service segment-based patterns and one-shot completion |
| src/LineReceiver.h/.cpp | Assemble bounded lines and recover at delimiters |
| src/Duration.h | Validate decimal durations without hardware side effects |
| tests/ | Compile production components against simulated GPIO, clock and Serial |

Only StatusLed writes the LED pin. The controller chooses behavior through the driver's interface. Line reception knows nothing about GPIO or application commands. Numeric parsing validates a temporary result before the controller commits configuration. Firmware source uses fixed storage; the Arduino core's internal implementation is separate.

## First useful run

Send commands separately:

```text
version
set on 2000
set off 3000
mode heartbeat
status
stop
status
blink
stop
errors
```

Observe whether stop ends the long ON phase without waiting for the five-second pattern. Then try burst and alert. Use `set on bad` and `set off 5001` to confirm rejection preserves the working settings. Restore 200/800 if desired.

## Repeatable checks

From the repository root, with Python 3 and g++ available:

```bash
python3 projects/device_controller/tests/run_tests.py
```

The suite covers numeric boundaries, malformed/huge numbers, partial lines, CR+LF, buffer limits and recovery, clock rollover, pattern durations, late servicing, cancellable long pulses, one-shot counting, timing-restart policy and manual overrides. It compiles the production C++ files with mocked hardware. It does not build the Arduino framework, open USB, or prove physical behavior.

Record full build/upload results and physical checks in [HARDWARE_CHECKS.md](HARDWARE_CHECKS.md). All physical results begin as not run. Earlier lesson test scripts expect their own version/protocol and should not be used unchanged against this project.
