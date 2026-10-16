"""Offline checks for the serial test harness, not connected-device acceptance."""
import unittest
from device_checks import Session, expect, fields

class FakePort:
    def __init__(self, data=b'', short_write=False):
        self.data = bytearray(data)
        self.sent = bytearray()
        self.short_write = short_write

    def read(self, count):
        result = bytes(self.data[:count])
        del self.data[:count]
        return result

    def write(self, payload):
        self.sent.extend(payload)
        return len(payload) - 1 if self.short_write else len(payload)

class HarnessTests(unittest.TestCase):
    def test_fields_and_variable_timestamp(self):
        record = fields('event=status captured_at_ms=123 led=OFF')
        expect(record, event='status', led='OFF')
        with self.assertRaises(AssertionError):
            expect(record, led='ON')

    def test_bytewise_response_and_transcript(self):
        port = FakePort(b'event=notice\r\nevent=status led=OFF\r\n')
        transcript = []
        record = Session(port, transcript).command('status', 'status', led='OFF')
        self.assertEqual(record['led'], 'OFF')
        self.assertEqual(port.sent, b'status\n')
        self.assertEqual(len(transcript), 3)

    def test_timeout_is_finite(self):
        with self.assertRaises(TimeoutError):
            Session(FakePort(), []).receive('status', timeout=0)

    def test_line_bound(self):
        with self.assertRaises(RuntimeError):
            Session(FakePort(b'x' * 4097), []).receive('status')

    def test_payload_bound(self):
        with self.assertRaises(ValueError):
            Session(FakePort(), []).send('x' * 257)

    def test_partial_write(self):
        with self.assertRaises(RuntimeError):
            Session(FakePort(short_write=True), []).send('status\n')

if __name__ == '__main__':
    unittest.main()
