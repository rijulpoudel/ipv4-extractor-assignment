"""Check the assignment's exact interactive output, not only the parser result."""
import subprocess
import sys
import unittest

PROGRAM = sys.argv.pop(1) if len(sys.argv) > 1 else "./ipv4"
PROMPT = "Enter a string (or 'END' to quit): "
INVALID = "Invalid input: no valid IPv4 address found\n"


class ConsoleTests(unittest.TestCase):
    def run_program(self, text):
        completed = subprocess.run([PROGRAM], input=text, text=True,
                                   capture_output=True, check=True)
        self.assertEqual(completed.stderr, "")
        return completed.stdout

    def test_sample_and_exit(self):
        actual = self.run_program("connecting to 192.168.1.1 now\n"
                                  "server=10.0.0.255:8080end\n"
                                  "192.168.1.1.\nEND\n")
        self.assertEqual(actual, PROMPT
                         + "Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)\n"
                         + PROMPT
                         + "Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)\n"
                         + PROMPT + INVALID + PROMPT + "Program terminated.\n")

    def test_end_is_case_sensitive(self):
        self.assertEqual(self.run_program("end\nEND\n"),
                         PROMPT + INVALID + PROMPT + "Program terminated.\n")

    def test_eof_exits_without_fake_end_message(self):
        self.assertEqual(self.run_program(""), PROMPT)


if __name__ == "__main__":
    unittest.main()
