
#pragma once

#include <nemcpp/utils/buffer.h>

#include <array>
#include <string>
#include <string_view>
#include <vector>

namespace xpx_sdk {
	
	/// Base32 encoder.
	class Base32 {
	public:
		/// Encodes data in \a buffer.
		static std::string Encode(RawBuffer buffer);
		
		/// Encodes data in \a buffer and adds padding if necessary.
		static std::string EncodeWithPadding(RawBuffer buffer);
		
		/// Decodes \a encoded to a byte vector.
		/// \note Accepts both padded and non-padded representation.
		static bool Decode(std::string_view encoded, std::vector<uint8_t>& decoded);
		
		/// Decodes \a encoded to a fixed-size byte array.
		/// \note Accepts both padded and non-padded representation.
		template<size_t N>
		static bool Decode(std::string_view encoded, std::array<uint8_t, N>& decoded)
		{
			return DecodeToBuffer(encoded, decoded);
		}
		
	private:
		static bool DecodeToBuffer(std::string_view encoded, MutableRawBuffer decoded);
	};
}
