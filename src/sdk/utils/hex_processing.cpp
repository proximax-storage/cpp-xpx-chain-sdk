/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "hex_processing.h"
using namespace xpx_chain_sdk;

namespace xpx_chain_sdk {
	std::string int_to_hex(uint64_t id) {
		std::stringstream stream;
		stream << std::hex << id;
		std::string result(16, '0');
		for(int i = (int)stream.str().size() -1 , j = 15; i >= 0; i--,j--) {
			result[j] = stream.str()[i];
		}
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