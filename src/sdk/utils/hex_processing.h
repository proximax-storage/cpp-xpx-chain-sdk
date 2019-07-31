
#include <string>
#include <stdint.h>
#include <sstream>

namespace xpx_sdk {
    std::string int_to_hex(uint64_t id) {
        std::stringstream stream;
        stream << std::hex << id;
        std::string result(16, '0');
        std::copy(stream.str().rbegin(), stream.str().rend(), result.rbegin());
        return result;
    }

    uint64_t hex_to_int(const std::string &s) {
        uint64_t res = 0;
        uint64_t shift = 0;
        for (auto c : s) {
            if (isalpha(c)) { res |= (10 + c - 'A') << shift; }
            else { res |= (c - '0') << shift; }
            shift += 4;
        }
        return res;
    }
}