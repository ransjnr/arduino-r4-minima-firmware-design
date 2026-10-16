# Device controller hardware checks

Firmware: device-controller-1.0.0. Board: UNO R4 Minima.

- Date, board/core version and firmware version reply:
- Full Arduino CLI build: passed for arduino:renesas_uno:minima on 2026-10-17. Program: 42,104 / 262,144 bytes; global/static RAM: 4,080 / 32,768 bytes.
- Upload result and actual serial port:
- Monitor: 115200, Newline/LF:

| Check | Result and evidence |
|---|---|
| Reset defaults to manual/OFF, 200/800, zero counters | Not run |
| Manual on/off/toggle matches observed LED | Not run |
| Heartbeat and both fixed patterns visibly match specification | Not run |
| Stop during 2000 ms ON phase ends it before natural completion | Not run |
| Query during ON and OFF remains usable | Not run |
| One-shot finishes off and counts once | Not run |
| Cancelled/restarted one-shot does not count as completed | Not run |
| Valid setting policy restarts heartbeat/once; manual/burst/alert preserved | Not run |
| Invalid numbers and out-of-range values preserve settings | Not run |
| Exactly 63 content bytes fit; 64 bytes reject at terminator | Not run |
| Oversized suffix cannot execute; next fresh command works | Not run |
| CR/LF/CR+LF and blank lines produce no extra command | Not run |
| Clear errors preserves timing and action counts | Not run |
| Operation without Serial Monitor continues | Not run |
| Reset restores configuration and counters | Not run |

Host component tests and syntax checks do not populate these results. Record approximate visual response honestly; precise pulse timing and electrical output require suitable instruments. No hard real-time guarantee is claimed.
