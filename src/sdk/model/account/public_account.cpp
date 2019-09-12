/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/account/public_account.h>

namespace xpx_chain_sdk {
	
	PublicAccount::PublicAccount(const Key& publicKey, NetworkIdentifier networkId):
		publicKey_(publicKey),
		networkId_(networkId),
		address_(this)
	{ }
	
	PublicAccount::PublicAccount(const PublicAccount& rhs):
		publicKey_(rhs.publicKey_),
		networkId_(rhs.networkId_),
		address_(this)
	{
		address_ = rhs.address_;
	}
	
	PublicAccount::PublicAccount(PublicAccount&& rhs) noexcept:
		publicKey_(std::move(rhs.publicKey_)),
		networkId_(rhs.networkId_),
		address_(this)
	{
		address_ = std::move(rhs.address_);
	}
	
	NetworkIdentifier PublicAccount::networkId() const
	{
		return networkId_;
	}
	
	const Key& PublicAccount::publicKey() const
	{
		return publicKey_;
	}
	
	const Address& PublicAccount::address() const
	{
		return *address_;
	}
	
	void PublicAccount::initAddress(std::optional<Address>& value)
	{
		value.emplace(publicKey_, networkId_);
	}
	
	bool operator==(const PublicAccount& lhs, const PublicAccount& rhs)
	{
		return lhs.networkId() == rhs.networkId() && lhs.publicKey() == rhs.publicKey();
	}
	
	bool operator!=(const PublicAccount& lhs, const PublicAccount& rhs)
	{
		return !(lhs == rhs);
	}
}
