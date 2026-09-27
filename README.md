# IPv4 extractor

A C++17 console program that finds the first valid IPv4 address, with optional port, in a line of noisy text. Each uninterrupted run of digits, periods, and colons must match the **entire** address grammar; a malformed longer run cannot supply a shorter match.

## Build and run

```sh
g++ -std=c++17 -Wall -Wextra -Werror -pedantic ipv4.cpp -o ipv4
./ipv4
```

Type `END` on its own line to exit. For example, `server=10.0.0.255:8080end` prints `Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)`.

## Test

```sh
g++ -std=c++17 -Wall -Wextra -Werror -pedantic -DIPV4_TEST ipv4.cpp test_ipv4.cpp -o test_ipv4
./test_ipv4
python3 test_cli.py ./ipv4
```

`test_ipv4.cpp` covers boundaries, malformed octets/ports, adjacent punctuation, garbage delimiters, and output reset on failure. `test_cli.py` checks exact console formatting and the case-sensitive `END` loop. The test-only compile flag excludes `main` from `ipv4.cpp` so the required function can be tested directly.

See [AI_DISCLOSURE.md](AI_DISCLOSURE.md) for the exact assignment prompt, authorship, test record, and review checklist.
