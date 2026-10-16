# Author verification — Lesson 03

Checked while preparing these exercises:

- Pure parser contract tests passed for all seven self-contained parser headers (Exercises 04–10), including numeric values 0..10000, malformed/huge tokens, output preservation, and leading zeros.
- ARM cross-compiler syntax checks passed for all ten sketches using the locally installed Minima core headers. These were syntax checks, not full Arduino preprocessing/linking builds.
- Native C++ firmware models passed normal command checks, the intentional Exercise 03 defect expectation, shared interface smoke checks for Exercises 06–10, timing/toggle paths, and Exercise 08's injected fault/repair path. Models used simulated GPIO/time and process pipes, not USB.
- Offline harness tests passed response parsing, stable-field assertions, finite timeout handling, payload/response bounds, and incomplete-write detection.
- Host-only regression ran successfully and explicitly reported device checks not run.
- Python syntax and document links were checked.

Not performed: full Arduino target builds, uploads, actual USB integration runs, physical resets, electrical measurements, or observed LED acceptance. Evidence templates intentionally contain no hardware passes. Exercise 03 ships with its declared defect; Exercise 08 ships with fault injection disabled. Learner extensions and deliberately introduced regressions remain tasks to implement/test, not completed work.

Repeat available checks from the project root:

```bash
python3 exercises/lesson_03/tools/run_host_checks.py
python3 exercises/lesson_03/tools/test_tooling.py
python3 exercises/lesson_03/tools/regression.py
```

Generated reports are under build/lesson03 and ignored by Git. The native model and ARM author checks do not replace the learner's physical evidence record.
