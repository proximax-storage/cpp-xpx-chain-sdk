
#pragma once

#include <nemcpp/exceptions.h>
#include <nemcpp/utils/hashable_array.h>

#include <cstdint>
#include <functional>
#include <type_traits>

namespace nem2_sdk {
	
	constexpr size_t Hash160_Size = 20;
	constexpr size_t Hash256_Size = 32;
	constexpr size_t Hash512_Size = 64;
	constexpr size_t Key_Size = 32;
	constexpr size_t Signature_Size = 64;
	constexpr size_t Address_Size = 25;
	constexpr size_t Address_Encoded_Size = 40;
	
	using Hash160 = HashableArray<uint8_t, Hash160_Size>;
	using Hash256 = HashableArray<uint8_t, Hash256_Size>;
	using Hash512 = HashableArray<uint8_t, Hash512_Size>;
	using Key = HashableArray<uint8_t, Key_Size>;
	using Signature = HashableArray<uint8_t, Signature_Size>;
	using AddressData = HashableArray<uint8_t, Address_Size>;
	
	using MosaicId = uint64_t;
	using Amount = uint64_t;
	
	using BlockDuration = uint64_t;
	constexpr BlockDuration Eternal_Duration = 0;
	
	using NamespaceId = uint64_t;
	constexpr NamespaceId Namespace_Base_Id = 0;
	
	/// Blockchain network identifier.
	enum class NetworkIdentifier: uint8_t {
		Unknown = 0x00,
		Mijin = 0x60,
		Mijin_Test = 0x90,
		Public = 0x68,
		Public_Test = 0x98
	};
	
	template<typename... TArgs>
	using predicate = std::function<bool (TArgs...)>;
	
	template<typename T>
	using supplier = std::function<T ()>;
	
	template<typename TEnum>
	constexpr typename std::underlying_type<TEnum>::type to_underlying_type(TEnum value)
	{
		return static_cast<std::underlying_type_t<TEnum>>(value);
	}
}
