/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#pragma once

#include <xpxchaincpp/exceptions.h>
#include <stdint.h>

namespace xpx_chain_sdk {
	/// Parses two characters (\a ch1 and \a ch2) into a byte.
	uint8_t ParseByte(char, char);

	/// Tries to parse two characters (\a ch1 and \a ch2) into a byte (\a by).
	bool TryParseByte(char ch1, char ch2, uint8_t& by);

	/// Tries to parse a hex string (\a pHexData with size \a dataSize) into \a outputContainer.
	template<typename TContainer>
	bool TryParseHexStringIntoContainer(const char* const pHexData, size_t dataSize, TContainer& outputContainer) {
		if (2 * outputContainer.size() != dataSize)
			return false;

		for (auto i = 0u; i < dataSize; i += 2) {
			if (!TryParseByte(pHexData[i], pHexData[i + 1], outputContainer[i / 2]))
				return false;
		}

		return true;
	}

	/// Parses a hex string (\a pHexData with size \a dataSize) into \a outputContainer.
	template<typename TContainer>
	void ParseHexStringIntoContainer(const char* const pHexData, size_t dataSize, TContainer& outputContainer) {
		// do not implement in terms of TryParseHexStringIntoContainer in order to provide better exception messages
		if (2 * outputContainer.size() != dataSize)
			XPX_CHAIN_SDK_THROW_1(serialization_error, "hex string has unexpected size", dataSize);

		for (auto i = 0u; i < dataSize; i += 2)
			outputContainer[i / 2] = ParseByte(pHexData[i], pHexData[i + 1]);
	}

	/// Parses a hex string (\a hexString) into a byte array.
	template<typename TByteArray>
	TByteArray ParseByteArray(const std::string& hexString) {
		TByteArray array;
		ParseHexStringIntoContainer(hexString.c_str(), hexString.size(), array);
		return array;
	}
}
