#!/usr/bin/env python3
"""Compile the production components against simulated GPIO/clock/Serial."""
from pathlib import Path
import shutil
import subprocess
import tempfile

project = Path(__file__).resolve().parents[1]
compiler = shutil.which('g++')
if not compiler:
    raise SystemExit('Blocked: install g++ to run host component checks')
with tempfile.TemporaryDirectory(prefix='device-controller-tests-') as directory:
    binary = Path(directory) / 'controller-tests'
    command = [compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror',
               '-I', str(project / 'tests/fakes'), '-I', str(project / 'src'),
               str(project / 'tests/test_controller.cpp'),
               *map(str, sorted((project / 'src').glob('*.cpp'))), '-o', str(binary)]
    subprocess.run(command, check=True, timeout=30)
    subprocess.run([str(binary)], check=True, timeout=10)
print('Host simulation only: GPIO, USB, real-time latency and physical LED remain unverified.')
