#!/usr/bin/env python3
"""Run pure parser checks for every self-contained Lesson 03 parser copy."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[3]
BASE = ROOT / 'exercises/lesson_03'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, default=ROOT / 'build/lesson03/host.json')
    args = parser.parse_args()
    report = {'layer': 'host_cpp_parser', 'hardware_tested': False, 'results': []}
    compiler = shutil.which('g++')
    code = 0
    if not compiler:
        report.update(outcome='blocked', reason='g++ not found')
        code = 2
    else:
        for header in sorted(BASE.glob('*/logic/duration.h')):
            with tempfile.TemporaryDirectory() as directory:
                binary = Path(directory) / 'parser_tests'
                commands = [[compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
                             '-DDURATION_HEADER="' + str(header) + '"',
                             str(BASE / '05_host_checks/parser_tests.cpp'), '-o', str(binary)],
                            [str(binary)]]
                entry = {'header': str(header.relative_to(ROOT)), 'outcome': 'pass', 'logs': []}
                for command in commands:
                    try:
                        result = subprocess.run(command, capture_output=True, text=True, timeout=30)
                        entry['logs'].append(result.stdout + result.stderr)
                        if result.returncode:
                            entry['outcome'] = 'fail'
                            code = 1
                            break
                    except (OSError, subprocess.TimeoutExpired) as error:
                        entry.update(outcome='blocked', reason=str(error))
                        code = 2 if code == 0 else code
                        break
                report['results'].append(entry)
        if not report['results']:
            code = 2
            report['reason'] = 'No parser headers found'
        report['outcome'] = 'pass' if code == 0 else ('fail' if code == 1 else 'blocked')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(f"Host parser checks: {report['outcome']}; report: {args.output}")
    return code

if __name__ == '__main__':
    raise SystemExit(main())
