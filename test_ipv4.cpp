#include <iostream>
#include <string>

// Exercise the required public function without changing its signature.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);

struct TestCase {
    const char* name;
    const char* input;
    bool found;
    unsigned long address;
    int port;
};

int main() {
    const TestCase cases[] = {
        {"plain address", "connecting to 192.168.1.1 now", true, 3232235777UL, -1},
        {"address and port", "server=10.0.0.255:8080end", true, 167772415UL, 8080},
        {"garbage splits candidates", "192a168.1.1.1", true, 2818638081UL, -1},
        {"trailing dot rejects entire token", "192.168.1.1.", false, 0UL, -1},
        {"maximum address and port", "255.255.255.255:65535", true, 4294967295UL, 65535},
        {"minimum address and port", "0.0.0.0:0", true, 0UL, 0},
        {"maximum octet", "255.0.0.1", true, 4278190081UL, -1},
        {"port without trailing punctuation", "1.2.3.4:9", true, 16909060UL, 9},
        {"invalid then valid candidate", "256.1.2.3 junk 1.2.3.4", true, 16909060UL, -1},
        {"first valid candidate wins", "1.2.3.4 and 5.6.7.8", true, 16909060UL, -1},
        {"address before text", "1.2.3.4end", true, 16909060UL, -1},
        {"text before address", "prefix1.2.3.4", true, 16909060UL, -1},
        {"empty input", "", false, 0UL, -1},
        {"no numbers", "no number here", false, 0UL, -1},
        {"truncated address", "12.34.56", false, 0UL, -1},
        {"extra octet", "1.2.3.4.5", false, 0UL, -1},
        {"leading dot", ".1.2.3.4", false, 0UL, -1},
        {"double dot", "1..2.3.4", false, 0UL, -1},
        {"octet too long", "1234.2.3.4", false, 0UL, -1},
        {"octet out of range", "256.1.2.3", false, 0UL, -1},
        {"octet leading zero", "192.168.01.1", false, 0UL, -1},
        {"zero octet leading zero", "00.1.2.3", false, 0UL, -1},
        {"empty port", "1.2.3.4:", false, 0UL, -1},
        {"port too long", "1.2.3.4:00000", false, 0UL, -1},
        {"port six digits", "1.2.3.4:123456", false, 0UL, -1},
        {"port above limit", "1.2.3.4:65536", false, 0UL, -1},
        {"large port", "1.2.3.4:99999", false, 0UL, -1},
        {"port leading zero", "1.2.3.4:01", false, 0UL, -1},
        {"port leading zeros", "1.2.3.4:000", false, 0UL, -1},
        {"second colon", "1.2.3.4:80:90", false, 0UL, -1},
        {"colon before address", ":1.2.3.4", false, 0UL, -1},
        {"colon in address", "1.2:3.4", false, 0UL, -1},
        {"period after port", "1.2.3.4:80.", false, 0UL, -1},
        {"period before port", "1.2.3.4.:80", false, 0UL, -1},
        {"port text split", "1.2.3.4:80end", true, 16909060UL, 80},
        {"malformed run followed by valid", "1.2.3.4.5 a 10.0.0.1", true, 167772161UL, -1},
        {"candidate stays whole after invalid port", "1.2.3.4:99999", false, 0UL, -1},
    };

    int failures = 0;
    for (const auto& test : cases) {
        // Failure must replace stale output values, not merely return false.
        unsigned long address = 123456UL;
        int port = 123;
        const bool found = extractIPv4(test.input, address, port);
        if (found != test.found || address != test.address || port != test.port) {
            std::cerr << "FAIL: " << test.name << " (found=" << found
                      << ", address=" << address << ", port=" << port << ")\n";
            ++failures;
        }
    }
    std::cout << (sizeof(cases) / sizeof(cases[0]) - failures)
              << "/" << sizeof(cases) / sizeof(cases[0]) << " parser cases passed\n";
    return failures == 0 ? 0 : 1;
}
