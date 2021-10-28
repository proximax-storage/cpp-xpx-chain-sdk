/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/exceptions.h>
#include <xpxchaincpp/traits.h>
#include <xpxchaincpp/utils/hashable_array.h>

#include <cstdint>
#include <functional>

namespace xpx_chain_sdk {
	
	constexpr size_t Hash160_Size = 20;
	constexpr size_t Hash256_Size = 32;
	constexpr size_t Hash512_Size = 64;
	constexpr size_t Key_Size = 32;
	constexpr size_t Signature_Size = 64;
	constexpr size_t Address_Size = 25;
	constexpr size_t Address_Encoded_Size = 40;
	constexpr size_t BLS_Public_Key_Size = 48;
	
	using Hash160 = HashableArray<uint8_t, Hash160_Size>;
	using Hash256 = HashableArray<uint8_t, Hash256_Size>;
	using Hash512 = HashableArray<uint8_t, Hash512_Size>;
	using Key = HashableArray<uint8_t, Key_Size>;
	using Signature = HashableArray<uint8_t, Signature_Size>;
	using AddressData = HashableArray<uint8_t, Address_Size>;
	using BLSPublicKey = HashableArray<uint8_t, BLS_Public_Key_Size>;
	
	using MosaicId = uint64_t;
	using Amount = uint64_t;
	
	using BlockDuration = uint64_t;
	constexpr BlockDuration Eternal_Duration = 0;
	
	using NamespaceId = int64_t;
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
}
