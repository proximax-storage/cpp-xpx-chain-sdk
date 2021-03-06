/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/config.h>
#include <xpxchaincpp/utils/hashable_array.h>
#include <xpxchaincpp/utils/lazy.h>

#include <string>
#include <string_view>
#include <typeindex> // provides 'hash' declaration and is much cheaper to include than <functional>

namespace xpx_chain_sdk {
	
	/// Client address.
	class Address {
	public:
		/// Creates address from binary \a addressData.
		/// \note Throws \c account_error if \a addressData is invalid (has incorrect network id or checksum).
		Address(const AddressData& addressData);
		
		/// Creates address from \a publicKey for a network \a networkId.
		explicit Address(const Key& publicKey, NetworkIdentifier networkId = GetConfig().NetworkId);
		
		/// Creates address from base32-encoded representation \a encodedAddress.
		/// \note Throws \c account_error if \a encoded is malformed.
		/// \note Accepts both strict base32 and pretty-print version of encoded address.
		explicit Address(std::string_view encodedAddress);
		
		/// Copy constructor.
		Address(const Address& rhs);
		
		/// Move constructor.
		Address(Address&& rhs) noexcept;
		
		/// Default copy assignment operator.
		Address& operator=(const Address&) = default;
		
		/// Default move assignment operator.
		Address& operator=(Address&&) = default;
		
		/// Returns address network identifier.
		NetworkIdentifier networkId() const;
		
		/// Returns binary address.
		const AddressData& binary() const;
		
		/// Returns base32-encoded address.
		const std::string& encoded() const&;

		/// Returns base32-encoded address.
		std::string&& encoded() &&;

		/// Returns pretty-print address.
		const std::string& pretty() const&;

		/// Returns pretty-print address.
		std::string&& pretty() &&;

		/// Returns binary address.
		const uint8_t* data() const;
		
		/// Returns binary address size.
		size_t size() const;
		
	public:
		/// Returns \c true if \a addressData is valid.
		static bool IsValid(const AddressData& addressData);
		
		/// Returns \c true if \a encodedAddress is valid base32-encoded address.
		/// \note Accepts both strict base32 and pretty-print version of encoded address.
		static bool IsValidEncoded(std::string_view encodedAddress);
		
	private:
		// Decodes base32-encoded string to address and performs all checks.
		static bool DecodeAndCheck(std::string_view encodedAddress, AddressData& addressData);
		
		void initEncodedAddress(std::optional<std::string>& value);
		void initPrettyAddress(std::optional<std::string>& value);
		
	private:
		AddressData data_;
		Lazy<std::string, Address, &Address::initEncodedAddress> encoded_;
		Lazy<std::string, Address, &Address::initPrettyAddress> pretty_;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const Address& lhs, const Address& rhs);
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const Address& lhs, const Address& rhs);
}

namespace std {
	
	template<>
	struct hash<xpx_chain_sdk::Address> {
		size_t operator()(const xpx_chain_sdk::Address& address) const
		{
			return std::hash<xpx_chain_sdk::AddressData>{}(address.binary());
		}
	};
}
