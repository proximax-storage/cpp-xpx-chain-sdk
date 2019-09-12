/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/utils/base32.h>

#include <cctype>

namespace xpx_chain_sdk {
	
	namespace {
		const char* Base32_Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		const char Base32_Padding = '=';
		constexpr uint8_t Base32_Block_Size = 5; // in bits
		
		std::string_view TrimPadding(std::string_view encoded)
		{
			size_t resultSize = encoded.size();
			for (; resultSize > 0 && encoded[resultSize - 1] == Base32_Padding; --resultSize);
			return encoded.substr(0, resultSize);
		}
		
		size_t GetBase32DecodedSize(size_t sizeWithoutPadding)
		{
			return (sizeWithoutPadding * Base32_Block_Size) / 8;
		}
		
		std::string Base32Encode(RawBuffer buffer, bool usePadding)
		{
			if (buffer.empty()) {
				return std::string{};
			}
			
			// Calculating size of encoded data.
			size_t encodedSize = (buffer.size() * 8) / Base32_Block_Size;
			
			if ((buffer.size() * 8) % Base32_Block_Size) {
				encodedSize += 1;
				
				if (usePadding) {
					encodedSize += 8 - encodedSize % 8; // padding should complement to 8-byte boundary
				}
			}
			
			// Encoding data.
			std::string encoded(encodedSize, '='); // unnecessary padding will be overwritten during encoding
			uint32_t blocksBuffer = buffer[0];
			
			uint32_t bitsLeft = 8;
			uint32_t encodedIndex = 0;
			uint32_t nextIndex = 1;
			
			while (nextIndex < buffer.size() || bitsLeft > 0) {
				if (bitsLeft < Base32_Block_Size) {
					if (nextIndex < buffer.size()) {
						blocksBuffer <<= 8;
						blocksBuffer |= 0xFF & buffer[nextIndex++];
						bitsLeft += 8;
					} else {
						blocksBuffer <<= Base32_Block_Size - bitsLeft;
						bitsLeft = Base32_Block_Size;
					}
				}
				
				encoded[encodedIndex++] = Base32_Alphabet[0x1F & (blocksBuffer >> (bitsLeft - Base32_Block_Size))];
				bitsLeft -= Base32_Block_Size;
			}
			
			return encoded;
		}
		
		bool Base32Decode(std::string_view encoded, MutableRawBuffer decoded)
		{
			uint32_t blocksBuffer = 0;
			uint32_t bitsLeft = 0;
			uint32_t decodedIndex = 0;
			
			for (size_t i = 0; i < encoded.size(); ++i) {
				char ch = encoded[i];
				blocksBuffer <<= Base32_Block_Size;
				
				if (ch >= 'A' && ch <= 'Z') {
					ch = (ch & 0x1F) - 1;
				} else if (ch >= '2' && ch <= '7') {
					ch -= static_cast<int>('2') - 26;
				} else {
					return false;
				}
				
				blocksBuffer |= ch;
				bitsLeft += Base32_Block_Size;
				
				if (bitsLeft >= 8) {
					decoded[decodedIndex++] = blocksBuffer >> (bitsLeft - 8);
					bitsLeft -= 8;
				}
			}
			
			return true;
		}
	}
	
	std::string Base32::Encode(RawBuffer buffer)
	{
		return Base32Encode(buffer, false);
	}
	
	std::string Base32::EncodeWithPadding(RawBuffer buffer)
	{
		return Base32Encode(buffer, true);
	}
	
	bool Base32::Decode(std::string_view encoded, std::vector<uint8_t>& decoded)
	{
		auto encodedWithoutPadding = TrimPadding(encoded);
		decoded.resize(GetBase32DecodedSize(encodedWithoutPadding.size()));
		return Base32Decode(encodedWithoutPadding, decoded);
	}
	
	bool Base32::DecodeToBuffer(std::string_view encoded, MutableRawBuffer decoded)
	{
		auto encodedWithoutPadding = TrimPadding(encoded);
		
		if (decoded.size() != GetBase32DecodedSize(encodedWithoutPadding.size())) {
			return false;
		}
		
		return Base32Decode(encodedWithoutPadding, decoded);
	}
}
