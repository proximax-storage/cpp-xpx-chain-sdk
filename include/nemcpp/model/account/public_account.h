
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/config.h>
#include <nemcpp/utils/hasher.h>
#include <nemcpp/utils/lazy.h>
#include <nemcpp/model/account/address.h>

#include <typeindex> // provides 'hash' declaration and is much cheaper to include than <functional>

namespace xpx_sdk {
	
	/// Blockchain public account.
	class PublicAccount {
	public:
		/// Creates public account from \a publicKey for \a networkId.
		PublicAccount(const Key& publicKey, NetworkIdentifier networkId = GetConfig().NetworkId);
		
		/// Copy constructor.
		PublicAccount(const PublicAccount& rhs);
		
		/// Move constructor.
		PublicAccount(PublicAccount&& rhs) noexcept;
		
		/// Default copy assignment operator.
		PublicAccount& operator=(const PublicAccount&) = default;
		
		/// Default move assignment operator.
		PublicAccount& operator=(PublicAccount&&) = default;
		
		/// Returns network id for which account was created.
		NetworkIdentifier networkId() const;
		
		/// Returns account public key.
		const Key& publicKey() const;
		
		/// Returns account address.
		const Address& address() const;
		
	private:
		void initAddress(std::optional<Address>& value);
		
	private:
		Key publicKey_;
		NetworkIdentifier networkId_;
		Lazy<Address, PublicAccount, &PublicAccount::initAddress> address_;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	/// \note Public accounts with the same public key but created for different networks are considered not equal.
	bool operator==(const PublicAccount& lhs, const PublicAccount& rhs);
	
	/// Returns \c true of \a lhs and \a rhs are not equal.
	bool operator!=(const PublicAccount& lhs, const PublicAccount& rhs);
}

namespace std {
	
	template<>
	struct hash<xpx_sdk::PublicAccount> {
		size_t operator()(const xpx_sdk::PublicAccount& publicAccount) const
		{
			using namespace xpx_sdk;
			using HasherType = compound_hasher<method_hasher<PublicAccount, NetworkIdentifier, &PublicAccount::networkId>,
			                                   method_hasher<PublicAccount, const Key&, &PublicAccount::publicKey>>;
			return HasherType{}(publicAccount);
		}
	};
}
