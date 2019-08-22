#pragma once

#include <array>
#include <vector>
#include <string>

namespace xpx_sdk {

	char num_to_char(int n) {
		if(n <= 9) return '0' + n;
		if(n <= 15) return 'A' + n - 10;
		return '.';
	}

	std::string bytes_to_string(const std::vector<uint8_t>& bytes) {
		std::string result;
		for(uint8_t byte : bytes) {
			result += num_to_char(byte & 15);
			byte >>= 4;
			result += num_to_char(byte & 15); // we could use just byte
		}
		return result;
	}

	template<int N>
	std::string bytes_to_string(const std::array<uint8_t, N>& bytes) {
		std::string result;
		for(uint8_t byte : bytes) {
			result += num_to_char(byte & 15);
			byte >>= 4;
			result += num_to_char(byte & 15); // we could use just byte
		}
		return result;
	}
}