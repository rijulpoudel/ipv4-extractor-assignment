#include <iostream>
#include <string>

// Validate one complete maximal run of digits, dots, and colons.
static bool parseCandidate(const std::string& text, std::size_t begin,
                           std::size_t end, unsigned long& address, int& port) {
    std::size_t pos = begin;
    unsigned long value = 0;

    // Consume exactly four decimal octets with exactly three separating dots.
    for (int octet = 0; octet < 4; ++octet) {
        const std::size_t firstDigit = pos;
        int part = 0;
        while (pos < end && text[pos] >= '0' && text[pos] <= '9') {
            if (pos - firstDigit == 3) return false;
            part = part * 10 + (text[pos] - '0');
            ++pos;
        }
        if (pos == firstDigit || (pos - firstDigit > 1 && text[firstDigit] == '0')
            || part > 255) return false;
        value = (value << 8) | static_cast<unsigned long>(part);
        if (octet < 3) {
            if (pos == end || text[pos] != '.') return false;
            ++pos;
        }
    }

    int parsedPort = -1;
    // A colon commits the entire candidate to having a fully valid port.
    if (pos < end && text[pos] == ':') {
        ++pos;
        const std::size_t firstDigit = pos;
        parsedPort = 0;
        while (pos < end && text[pos] >= '0' && text[pos] <= '9') {
            if (pos - firstDigit == 5) return false;
            parsedPort = parsedPort * 10 + (text[pos] - '0');
            ++pos;
        }
        if (pos == firstDigit || (pos - firstDigit > 1 && text[firstDigit] == '0')
            || parsedPort > 65535) return false;
    }

    // Extra digits, dots, or colons invalidate the *whole* candidate.
    if (pos != end) return false;
    address = value;
    port = parsedPort;
    return true;
}

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;
    std::size_t pos = 0;
    while (pos < str.size()) {
        // Non-token characters separate candidates rather than joining them.
        if ((str[pos] < '0' || str[pos] > '9') && str[pos] != '.' && str[pos] != ':') {
            ++pos;
            continue;
        }
        const std::size_t begin = pos;
        while (pos < str.size() && ((str[pos] >= '0' && str[pos] <= '9')
               || str[pos] == '.' || str[pos] == ':')) ++pos;
        if (parseCandidate(str, begin, pos, outAddress, outPort)) return true;
    }
    return false;
}

#ifndef IPV4_TEST
int main() {
    std::string line;
    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        if (!std::getline(std::cin, line)) break;
        if (line == "END") {
            std::cout << "Program terminated.\n";
            break;
        }
        unsigned long address = 0;
        int port = -1;
        if (!extractIPv4(line, address, port)) {
            std::cout << "Invalid input: no valid IPv4 address found\n";
            continue;
        }
        // Recover the four octets from their most-significant-to-least-significant packing.
        std::cout << "Extracted IPv4 address: "
                  << ((address >> 24) & 255UL) << '.' << ((address >> 16) & 255UL)
                  << '.' << ((address >> 8) & 255UL) << '.' << (address & 255UL)
                  << " (decimal value: " << address << ", port: ";
        if (port == -1) std::cout << "none";
        else std::cout << port;
        std::cout << ")\n";
    }
}
#endif
