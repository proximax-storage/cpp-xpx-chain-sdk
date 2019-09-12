/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/#pragma once

#include <array>
#include <vector>
#include <string>

namespace xpx_chain_sdk {

	unsigned char num_to_char(int n) {
		unsigned char result = ' ';
		if(n <= 9) result = '0' + n;
		else if(n <= 15) result = 'A' + n - 10;

		return result;
	}

	std::string bytes_to_string(const std::vector<uint8_t>& bytes) {
		std::string result;
		int n = bytes.size();
		result.reserve(n * 2);
		for(int i = 0; i < n; i++) {
			int byte = bytes[i];
			result.push_back(num_to_char(byte & 0xF));
			byte >>= 4;
			result.push_back(num_to_char(byte)); // we could use just byte
			std::swap(result.back(), result[result.size() - 2]);
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