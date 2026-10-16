# Lesson 01: your first firmware

## Goal

Build and explain a reliable LED signaling program through ten exercises. Learn the build/upload workflow, timing, C++ foundations, and systematic verification. No external wiring is needed.

## Exercise plan

Each exercise includes implementation, a prediction, hardware observation, and an explanation. All ten exercises have source and instructions. We have moved to Lesson 02; these exercises remain available for practice and reference. Hardware results must be recorded separately.

| Exercise | Build task | Completion check |
|---|---|---|
| 01 — First upload | Configure the board, upload Blink, change timing, and resolve upload errors. | Explain compile versus upload and identify your own firmware running. |
| 02 — Named timing | Replace raw durations with constants; compare equal and unequal on/off times. | Calculate period and duty cycle for three timing choices. |
| 03 — Signal pattern | Build a short-short-long signal using a function with duration parameters. | Trace every transition and account for the entire cycle. |
| 04 — Repeated pulses | Use a bounded `for` loop to generate a configurable burst of pulses. | Demonstrate one, three, and five pulses without duplicating calls. |
| 05 — Selectable patterns | Use a mode variable and conditions to select heartbeat, burst, or alert. | Test every valid mode and define a safe LED-off result for an invalid mode. |
| 06 — Changing state | Count completed bursts and alternate patterns after a fixed number of bursts. | Explain initialization, updates, scope, and reset behavior. |
| 07 — Timing sequences | Gradually change pulse duration between explicit limits, then restart the sequence. | Prevent zero/negative durations and explain unsigned arithmetic risks. |
| 08 — Separate startup from operation | Show a one-time startup signal, then enter a repeating operational pattern. | Confirm startup occurs once after reset and measure the operational cycle. |
| 09 — Diagnose deliberate faults | Investigate an incorrect duration, misplaced statement, and repetition-count error in copies of working code. | Record symptom, hypothesis, single change, and result for each fault. |
| 10 — LED status indicator | Combine startup, selectable modes, reusable functions, and bounded timing into one application. | Supply a behavior specification, timing table, and hardware test record; explain why blocking delays limit responsiveness. |

Keep earlier sketches intact. We will introduce and explain the code needed for each exercise as we reach it. Serial interaction and nonblocking timing belong to later phases.

## Exercise 01: first upload

## The pieces

- **Board:** the circuit board containing the microcontroller, connectors, and supporting components.
- **Microcontroller:** a small computer that executes instructions and interacts with hardware.
- **Firmware:** the program stored on the board that controls its behavior.
- **Sketch:** Arduino's name for a program; its source file ends in `.ino`.
- **Compile:** translate source code into machine instructions for the selected board.
- **Upload:** transfer the compiled program to the board.

Arduino sketches use C++ and the Arduino framework. The framework supplies functions such as `pinMode` and arranges for `setup()` to run once, followed by repeated calls to `loop()`.

## Read the sketch

Open `exercises/lesson_01/01_blink/01_blink.ino`.

`void setup()` defines a function named `setup` that returns no value. Its body is between `{` and `}`. `pinMode(LED_BUILTIN, OUTPUT);` configures the built-in LED's pin as an output. `LED_BUILTIN` is a board-provided name, so we do not need to hardcode its pin number.

`void loop()` defines the work repeated after setup. `digitalWrite` sets the output HIGH or LOW. On this board, that turns the built-in LED on or off. `delay(1000)` waits for 1000 milliseconds, or one second. While waiting, our sketch does not advance to its next statement; background framework activity can still occur.

Most statements end with `;`. Text after `//` is a comment for the reader.

One cycle takes about two seconds: one second on plus one second off. That is approximately 0.5 cycles per second (0.5 Hz), with a 50% duty cycle: the LED is on for half the cycle.

## Run it

1. Install/open Arduino IDE from https://www.arduino.cc/en/software/.
2. In Boards Manager, install **Arduino UNO R4 Boards**.
3. Connect the Minima using a USB-C cable that supports data.
4. Select **Arduino UNO R4 Minima** and its port in the IDE.
5. Open `exercises/lesson_01/01_blink/01_blink.ino`.
6. Click **Verify** to compile, then **Upload** to program the board.
7. Watch the built-in LED marked **L**.

The board may already blink before uploading. A successful upload and a deliberate timing change help establish that your own program is running.

## Experiment

Before changing the code, predict the result. Then change only the first delay to `200` and the second to `1800`, and upload again.

Record:

- How long is the LED on?
- How long is it off?
- How long is a complete cycle?
- What fraction of the cycle is it on?

Keep this modified sketch as your exercise result. Explain why changing a delay does not require changing `setup()`.

## Completion check

You should be able to explain the difference between compiling and uploading, the roles of `setup()` and `loop()`, and why two 1000 ms delays produce a two-second cycle.

The learner reported successful upload and timing changes on 2026-10-16. See the notebook for the resolved USB permissions error. The saved sketch now uses 500 ms for each delay; this exact revision has not been separately confirmed on hardware.

## Official references

- https://docs.arduino.cc/hardware/uno-r4-minima/
- https://docs.arduino.cc/built-in-examples/basics/Blink/


## Exercise 02: named timing values

Open `exercises/lesson_01/02_named_timing/02_named_timing.ino`. This continues the same LED exercise with named constants.

### Start from the saved behavior

The saved Lesson 01 sketch uses `delay(500)` twice. Our new sketch starts with the same timing, but gives each duration a name:

```cpp
const unsigned long onTimeMs = 500;
const unsigned long offTimeMs = 500;
```

Read the first declaration:

- `const`: the value cannot be reassigned after initialization.
- `unsigned long`: an integer type for nonnegative whole numbers.
- `onTimeMs`: our chosen name. `Ms` communicates milliseconds; it does not convert units for us.
- `= 500`: initialize the value to 500.
- `;`: end the declaration.

These are constants. Later, we will introduce variables whose values change while the program runs.

### Use the names

`delay(onTimeMs)` uses the value 500. Naming the duration explains what it means and gives us a clear place to change it. The initial behavior is still half a second on and half a second off.

The declarations are outside both functions, giving the names global scope. Both functions can refer to them after their declarations. Braces `{}` enclose each function body. We will explore local scope with functions later.

### Practice

1. Open the new sketch and verify/upload it. Confirm the initial timing.
2. Change only the two constant values to make the LED stay on for half a second and off for one and a half seconds.
3. Predict the complete cycle duration before uploading again.
4. Upload, observe, and record your values and result in `notes/lab-notebook.md`.

### Explain before moving on

- What does `const` prevent?
- Why did we include `Ms` in each name?
- Does naming the values change the LED behavior by itself?
- What is the total cycle duration after your change?

Status: prepared; compilation and hardware behavior are not yet verified.


## Exercise 03: a reusable LED signal

Read [the Exercise 03 guide](../exercises/lesson_01/03_signal_pattern/README.md), then open its sketch. This moves from individual blinks to a specified pattern and introduces functions and parameters.


## Exercise 04: configurable pulse bursts

Read [the Exercise 04 guide](../exercises/lesson_01/04_pulse_bursts/README.md) and open its sketch. Use a `for` loop to repeat pulses, trace the counter, diagnose an off-by-one error, and design a burst with a specified total period.


## Exercise 05: selectable LED patterns

Read [the Exercise 05 guide](../exercises/lesson_01/05_selectable_patterns/README.md) and open its sketch. Select patterns with conditions, test the invalid-mode fallback, investigate assignment versus comparison, and implement a fourth mode.


## Exercise 06: changing state

Read [the Exercise 06 guide](../exercises/lesson_01/06_changing_state/README.md) and open its sketch. Count complete patterns, switch modes automatically, investigate variable lifetime and counter resets, and implement separate phase lengths.


## Exercise 07: timing sequences

Read [the Exercise 07 guide](../exercises/lesson_01/07_timing_sequences/README.md) and open its sketch. Shorten pulses between explicit limits, clamp the final step, test boundary configurations, and calculate a complete sequence.


## Exercise 08: startup versus operation

Read [the Exercise 08 guide](../exercises/lesson_01/08_startup_operation/README.md) and open its sketch. Separate a one-time startup signature from normal heartbeat operation, investigate reset, and design your own startup signal.

## Exercise 09: diagnose deliberate faults

Read [the Exercise 09 guide](../exercises/lesson_01/09_diagnose_faults/README.md) and open its sketch. Establish a baseline, isolate three deliberate defects, record hypotheses and corrections, and retest shared configuration.

## Exercise 10: LED status indicator

Read [the Exercise 10 guide](../exercises/lesson_01/10_status_indicator/README.md) and open its sketch. Integrate startup, automatic mode transitions, validation, and reusable functions. Complete the timing analysis, refactor configuration, extend the modes, and record hardware acceptance.
