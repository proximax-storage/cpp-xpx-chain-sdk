#include "hex_processing.h"
#include <iostream>
using namespace xpx_sdk;

namespace xpx_sdk {
	std::string int_to_hex(uint64_t id) {
		std::stringstream stream;
		stream << std::hex << id;
		std::cout << id << ' ' << stream.str() << std::endl;
		std::string result(16, '0');
		std::cout << result << std::endl;
		for(int i = (int)stream.str().size() -1 , j = 15; i >= 0; i--,j--) {
			result[j] = stream.str()[i];
		}
		std::cout << "int_to_hex:" << result << std::endl;
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