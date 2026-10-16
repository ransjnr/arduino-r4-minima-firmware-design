#!/usr/bin/env python3
"""Opt-in, bounded serial checks. Never builds or uploads firmware."""
import argparse
import json
from pathlib import Path
import time

ROOT = Path(__file__).resolve().parents[3]

def fields(line):
    return dict(part.split('=', 1) for part in line.split() if '=' in part)

def expect(record, **wanted):
    for key, value in wanted.items():
        if record.get(key) != str(value):
            raise AssertionError(f'{key}: expected {value!r}, got {record.get(key)!r}; record={record}')

class Session:
    def __init__(self, port, transcript):
        self.port = port
        self.transcript = transcript
        self.pending = bytearray()

    def send(self, text):
        payload = text.encode('ascii')
        if len(payload) > 256:
            raise ValueError('Test payload too large')
        self.transcript.append({'direction': 'tx', 'host_s': time.monotonic(), 'text': text})
        count = self.port.write(payload)
        if count != len(payload):
            raise RuntimeError('Incomplete serial write')

    def receive(self, event, timeout=3.0):
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            chunk = self.port.read(1)
            if not chunk:
                continue
            self.pending.extend(chunk)
            if len(self.pending) > 4096:
                raise RuntimeError('Response line exceeds 4096 bytes')
            if chunk == b'\n':
                line = self.pending.decode('ascii', errors='replace').strip()
                self.pending.clear()
                self.transcript.append({'direction': 'rx', 'host_s': time.monotonic(), 'text': line})
                record = fields(line)
                if record.get('event') == event:
                    return record
        raise TimeoutError(f'No event={event} within {timeout} seconds')

    def command(self, text, event, timeout=3.0, **wanted):
        self.send(text + '\n')
        record = self.receive(event, timeout)
        expect(record, **wanted)
        return record

def smoke(session):
    session.command('test reset', 'test_reset')
    session.command('status', 'status', mode='manual', led='OFF', on_ms=200, off_ms=800,
                    completed_blinks=0, rejected_commands=0)
    session.command('on', 'on')
    session.command('status', 'status', led='ON')
    session.command('set on 400', 'setting_applied', name='on', value_ms=400)
    session.command('set off 600', 'setting_applied', name='off', value_ms=600)
    session.command('set on bad', 'error', reason='invalid_number')
    session.command('set off 49', 'error', reason='out_of_range')
    session.command('unknown', 'error', reason='unknown_command')
    session.command('x' * 31, 'error', reason='unknown_command')
    session.command('x' * 32 + 'on', 'error', reason='line_rejected')
    session.command('errors', 'errors', total=5, numeric=1, range=1, unknown=2, line=1)
    session.command('status', 'status', led='ON', on_ms=400, off_ms=600,
                    completed_blinks=0, rejected_commands=5)
    session.command('blink', 'blink_finished', timeout=4.0)
    session.command('status', 'status', led='OFF', completed_blinks=1)
    session.command('clear errors', 'errors_cleared')
    session.command('status', 'status', on_ms=400, off_ms=600, completed_blinks=1, rejected_commands=0)
    session.command('errors', 'errors', total=0, unknown=0, numeric=0, range=0, line=0, last='none')

def timing(session):
    session.command('test reset', 'test_reset')
    session.command('set on 200', 'setting_applied')
    session.command('set off 800', 'setting_applied')
    sent_at = time.monotonic()
    session.send('blink\nstatus\n')  # Deliberate short pipeline for responsiveness observation.
    session.receive('blink_started')
    session.receive('blink_finished', timeout=4.0)
    status = session.receive('status', timeout=3.0)
    expect(status, completed_blinks=1, led='OFF')
    return {'host_round_trip_s': time.monotonic() - sent_at,
            'nominal_block_ms': 1000,
            'interpretation': 'Includes host/USB/logging time; not precise command latency or electrical timing'}

def toggle(session):
    session.command('test reset', 'test_reset')
    session.command('toggle', 'toggled', led='ON')
    session.command('status', 'status', led='ON', completed_blinks=0, rejected_commands=0)
    session.command('toggle', 'toggled', led='OFF')
    session.command('status', 'status', led='OFF', completed_blinks=0, rejected_commands=0)
    session.command('toggle now', 'error', reason='unknown_command')

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', required=True)
    parser.add_argument('--expected-firmware', required=True)
    parser.add_argument('--suite', choices=['smoke', 'timing', 'toggle'], default='smoke')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/lesson03/device.json')
    args = parser.parse_args()
    report = {'layer': 'usb_integration', 'port': args.port, 'expected_firmware': args.expected_firmware,
              'suite': args.suite, 'physical_led_verified': False, 'transcript': []}
    result = 0
    port = None
    try:
        try:
            import serial
        except ImportError as error:
            raise OSError('pyserial missing; install tools/requirements.txt in your virtual environment') from error
        port = serial.Serial(args.port, 115200, timeout=0.1, write_timeout=2, exclusive=True)
        port.reset_input_buffer()
        session = Session(port, report['transcript'])
        version = None
        for _ in range(3):
            try:
                version = session.command('version', 'version', timeout=2.0)
                break
            except TimeoutError:
                pass
        if version is None:
            raise OSError('No version response; check uploaded sketch, port, and USB readiness')
        if version.get('firmware') != args.expected_firmware:
            raise OSError(f'Wrong fixture firmware: {version}')
        if args.suite == 'smoke':
            smoke(session)
        elif args.suite == 'timing':
            report['measurement'] = timing(session)
        else:
            toggle(session)
        session.command('test reset', 'test_reset')  # End with LED off and defaults.
        report['outcome'] = 'pass'
    except (AssertionError, TimeoutError, RuntimeError, ValueError) as error:
        report.update(outcome='fail', reason=str(error))
        result = 1
    except OSError as error:
        report.update(outcome='blocked', reason=str(error))
        result = 2
    finally:
        if port is not None:
            if result:
                try:
                    port.write(b'\noff\n')  # Best effort only; not proof the LED turned off.
                except OSError:
                    pass
            port.close()
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(f"Device checks: {report['outcome']}; report: {args.output}")
    return result

if __name__ == '__main__':
    raise SystemExit(main())
