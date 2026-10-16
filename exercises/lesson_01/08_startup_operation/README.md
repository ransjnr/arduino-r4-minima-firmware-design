# Exercise 08 — Startup versus operation

## Goal and behavior specification

Make startup visibly different from normal operation. Generate three quick flashes once, wait, then run a repeating heartbeat. No extra components are needed.

| Phase | Behavior | Nominal duration |
|---|---|---|
| Startup signal | Three pulses, each 100 ms on and 100 ms off | 600 ms |
| Startup pause | LED remains off | 1400 ms |
| Operation | Repeated 100 ms on, 1900 ms off | 2000 ms per cycle |

The first operational pulse begins nominally 2000 ms after entering the sketch's setup. The last startup falling edge occurs at 500 ms, so the gap from that edge to operation is 1500 ms. Bootloader activity before setup is not part of these calculations and may cause additional visible LED activity.

## Understand the structure

`setup()` configures the pin and performs startup work once per program start. `loop()` contains ongoing work. Resetting the board starts the program again, so setup runs again; calling loop repeatedly does not repeat setup.

`sendBurst` and `pulseLed` are function definitions. They can be called from either setup or loop. Reusing a helper does not make it run automatically. Its call site determines when and how often it executes.

The LED is initialized LOW before the first signal. Each pulse also ends LOW, making the pause and handoff to normal operation explicit. The startup pause blocks progress into operation, which is acceptable for this exercise but can be undesirable in a responsive product.

## Experiments

1. Trace the startup edges and first two operational pulses on paper. Verify/upload and observe startup followed by at least five heartbeat cycles. Ignore any separate bootloader activity before the sketch starts.
2. Press reset during normal operation. Predict which work repeats and observe. Repeat after disconnecting and reconnecting USB power. Record whether your observations distinguish startup from operation clearly.
3. Change the startup count to five while leaving heartbeat unchanged. Calculate the new startup duration and time to the first operational pulse. Verify on hardware.
4. Move only the startup signal and its pause from setup into the beginning of loop, temporarily. Predict the repeating combined pattern and its duration. Upload, observe, then restore the original separation.
5. Design a distinct startup signature: short-short-long, followed by a quiet gap. Use helpers rather than duplicating digitalWrite statements. Choose and document durations, calculate the first operational pulse time, and implement inside setup.
6. Keep your new startup signature and confirm that normal operation still uses the same heartbeat timing. Record final behavior and reset observations.

## Completion check

Explain why setup runs again after reset, how the helpers are shared, why startup does not belong inside the repeating loop for this specification, and which intervals count toward time to normal operation.

Status: source prepared; target compilation and hardware observations have not been confirmed.
