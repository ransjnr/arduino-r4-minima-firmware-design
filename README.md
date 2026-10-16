# Firmware design practice

Learn firmware development through practical exercises, starting with the Arduino UNO R4 Minima. The MXCHIP IoT DevKit will be used later.

## Learning path

1. First firmware: ten exercises in LED control, timing, C++ foundations, and verification.
2. Serial communication: observing program state, commands, and debugging.
3. Testing and diagnosing firmware behavior.
4. Timing without blocking; state machines.
5. GPIO inputs, buttons, and debouncing (when components are available).
6. Analog measurements and PWM (when components are available).
7. UART, I2C, and SPI; reading datasheets.
8. Interrupts, memory, error handling, and testing.
9. Organizing a larger firmware application.
10. Transfer the concepts to the MXCHIP board with its supported tooling.

We will introduce each topic when an experiment needs it. Progress depends on understanding and observed results.

## Start here

Lesson 01 upload and timing experiment worked, as reported during practice. The saved Lesson 01 sketch currently uses 500 ms on and 500 ms off; keep it as your experiment.

**Current work:** [Lesson 04 — Build a responsive device controller](lessons/04-firmware-project.md). Open `projects/device_controller/device_controller.ino` in VS Code; the workspace selects it for Verify/Upload. We now develop one application incrementally, starting with a nonblocking LED component and responsive command handling.

Lessons 01 and 02 each contain ten prepared exercises. Lesson 03 now has all ten exercises, host checks, and optional connected-board tools. Prepared source, host checks, target builds, uploads, and physical acceptance are distinct milestones; record actual results rather than treating preparation as completion.

Record your predictions, observations, and explanations in [the lab notebook](notes/lab-notebook.md).

## References

- [Arduino UNO R4 Minima](https://docs.arduino.cc/hardware/uno-r4-minima/)
- [Arduino Blink example](https://docs.arduino.cc/built-in-examples/basics/Blink/)
