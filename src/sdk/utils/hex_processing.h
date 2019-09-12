/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once
#include <string>
#include <stdint.h>
#include <sstream>

namespace xpx_chain_sdk {
    std::string int_to_hex(uint64_t id);
    uint64_t hex_to_int(const std::string &s);

}