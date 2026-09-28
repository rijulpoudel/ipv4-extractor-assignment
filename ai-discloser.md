## Tool and dates

I used ChatGPT (GPT-6-sol) across September 18-21, 2026, for the initial implementation and the follow-up revisions described below.

## Prompts I sent (exact text)

**1. Initial generation**

> Write a C++17 program that reads one line of text at a time and extracts a single valid IPv4 address with an optional port. Rules: only digits, periods, and colons can be part of a candidate token; any other character separates candidates. An address is four 1-3 digit octets separated by periods, each 0-255, with no leading zero unless the octet is exactly "0". An optional :port after the fourth octet is 1-5 digits, 0-65535, same leading-zero rule, and if a colon is present an invalid port must reject the address too. A candidate must match the grammar in full; do not accept a valid-looking piece of a longer run of digits, periods, and colons. Use this signature: bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort). Set outAddress to 0 and outPort to -1 on failure, and outPort to -1 when no port is present. Do not use atoi, stoi, strtol, sscanf, inet_pton, inet_aton, or any regex facility. Accumulate all digits by hand. main should loop with the prompt "Enter a string (or 'END' to quit): " until the user enters END, then print "Program terminated." On success print "Extracted IPv4 address: A.B.C.D (decimal value: N, port: P)" where P is the port or the literal text none. On failure print "Invalid input: no valid IPv4 address found".
> **2. Fixing partial matches (after my first tests failed)**
> Your scan returns a valid address for the input "1.2.3.4." and for "1.2.3.4:99999". The first must be invalid because the fourth octet is followed by more token characters, and the second must be invalid because a colon commits the match to requiring a fully valid port. Treat each maximal run of digits, periods, and colons as one candidate and reject the candidate unless it consumes the entire run. Change only this behavior.
> **3. Colon placement**
> Also reject the whole candidate if a colon appears anywhere other than immediately after the fourth octet, for example "1.2.3.4:80:90", ":1.2.3.4", and "1.2:3.4". Reject an empty port after a colon ("1.2.3.4:"), ports longer than five digits, and octets longer than three digits.
> **4. Leading zeros and numeric limits**
> Apply the leading-zero rule to the port as well as the octets: "1.2.3.4:01" and "1.2.3.4:000" must be invalid, only a single "0" is allowed. Verify the boundaries: 0.0.0.0 and 255.255.255.255:65535 are valid, 256.1.2.3 and 1.2.3.4:65536 are invalid.
> **5. Test cases**
> Write a set of test cases for extractIPv4 with expected results, covering normal addresses, garbage text around the address, partial and truncated addresses, adjacent punctuation, leading zeros, empty parts, oversized parts, and the numeric boundaries above. Print a pass count.

## What the AI wrote vs. what I wrote

- The AI generated the scanner, validator, and input loop from prompts 1-4. I reviewed each draft before accepting it.
- I wrote prompt 2 after a test I designed failed: `1.2.3.4.` was returning a valid address because the draft stopped at the end of the fourth octet without checking what followed. Prompt 3 came from similar tests of colon placement.
- I made these fixes by hand to the AI's output:
  - The port loop checked the five-digit limit after accumulating a digit, so a sixth digit still parsed. I moved the length check so it runs before accumulating.
  - The draft applied the leading-zero rule to octets but not to ports. I added that check.
- I rewrote the AI's draft test list into my own test file, chose the cases myself, and added the boundary and adjacent-punctuation cases.

## Testing and results

- Compiled with `g++ -std=c++17 -Wall -Wextra -Werror -pedantic`. No warnings.
- 44 test cases pass, covering: minimum and maximum values, leading zeros, missing and extra octets, empty ports, oversized ports, second colons, stray punctuation next to a valid address, garbage characters splitting tokens, and multiple candidates in one line.
- I also ran the transcript from the assignment's sample run; the output matches line for line.

## Verification statement

I understand every line of the code I submitted, including why each length check and boundary check exists, and I did not copy anything I could not explain. The code is tested and works as intended for the cases above.
Known limitations: if a line contains more than one valid address, the function returns the first one; the assignment assumes a single valid address per line. The spec text "no leading zero unless the value is exactly 0" could be read as allowing "00"; I reject it to match the sample's treatment of `192.168.01.1`, and I noted the interpretation here.
