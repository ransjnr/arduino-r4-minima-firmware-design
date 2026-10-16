#!/usr/bin/env python3
"""Run host checks and, only with an explicit port, connected-device checks."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
TOOLS = Path(__file__).resolve().parent

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port')
    parser.add_argument('--expected-firmware')
    parser.add_argument('--toggle', action='store_true')
    parser.add_argument('--output-dir', type=Path, default=ROOT / 'build/lesson03/regression')
    args = parser.parse_args()
    if bool(args.port) != bool(args.expected_firmware):
        parser.error('--port and --expected-firmware must be supplied together')
    if args.toggle and not args.port:
        parser.error('--toggle requires a connected-device run')
    args.output_dir.mkdir(parents=True, exist_ok=True)
    layers = []
    commands = [('host', [sys.executable, str(TOOLS / 'run_host_checks.py'), '--output', str(args.output_dir / 'host.json')])]
    if args.port:
        suites = ['smoke'] + (['toggle'] if args.toggle else [])
        for suite in suites:
            commands.append((suite, [sys.executable, str(TOOLS / 'device_checks.py'), '--port', args.port,
                                     '--expected-firmware', args.expected_firmware, '--suite', suite,
                                     '--output', str(args.output_dir / (suite + '.json'))]))
    for name, command in commands:
        try:
            result = subprocess.run(command, capture_output=True, text=True, timeout=120)
            layers.append({'layer': name, 'exit_code': result.returncode, 'output': result.stdout + result.stderr})
        except (subprocess.TimeoutExpired, OSError) as error:
            layers.append({'layer': name, 'exit_code': 2, 'output': str(error)})
    codes = [entry['exit_code'] for entry in layers]
    outcome = 'fail' if any(code not in (0, 2) for code in codes) else ('blocked' if 2 in codes else 'pass')
    report = {'outcome': outcome, 'device_requested': bool(args.port),
              'physical_led_verified': False, 'layers': layers,
              'device_note': 'Requested' if args.port else 'Not run: no port supplied; host pass is not device acceptance'}
    (args.output_dir / 'summary.json').write_text(json.dumps(report, indent=2) + '\n')
    print(f"Regression: {outcome}; device {'requested' if args.port else 'not run'}; {args.output_dir / 'summary.json'}")
    return 0 if outcome == 'pass' else (2 if outcome == 'blocked' else 1)

if __name__ == '__main__':
    raise SystemExit(main())
