# Lesson 04 — Build a responsive device controller

This phase develops one application rather than independent micro-exercises. The [device controller project](../projects/device_controller/README.md) now includes complete source for the Minima's built-in LED and USB interface: operating modes, nonblocking patterns, bounded line input, strict configuration validation, diagnostics, and component tests.

Read the interface before implementation: Config → StatusLed → LineReceiver → Controller → main sketch. Trace an incoming stop line through reception and command dispatch, then trace a timing tick through the output driver. Compare a cancelled one-shot with a completed one.

Build and upload through the selected VS Code sketch. Use 115200 and newline-terminated commands. Record physical results in the project's hardware checklist. Host simulations, target builds, and physical observations are separate evidence.

External input/peripheral drivers, direct GPIO register access, hardware timers, and interrupt handlers remain future extensions requiring the actual component and hardware requirements. The current application is complete for its stated built-in-LED/USB scope.
