#pragma once
#include <string>
#include <stdint.h>
#include <sstream>

namespace xpx_chain_sdk {
    std::string int_to_hex(uint64_t id);
    uint64_t hex_to_int(const std::string &s);

}