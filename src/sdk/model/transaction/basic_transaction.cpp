/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/basic_transaction.h>
#include <xpxchaincpp/config.h>

#include "sdk/model/transaction/transaction_utils.h"

namespace xpx_chain_sdk {
	
	BasicTransaction::BasicTransaction(TransactionType type, uint32_t fullVersion, const std::optional<Key>& signer):
		signer_(this, signer),
		version_(internal::ExtractTransactionVersion(fullVersion)),
		networkId_(internal::ExtractTransactionNetworkId(fullVersion)),
		type_(type)
	{ }
	
	BasicTransaction::BasicTransaction(const BasicTransaction& rhs):
		signer_(this),
		version_(rhs.version_),
		networkId_(rhs.networkId_),
		type_(rhs.type_)
	{
		signer_ = rhs.signer_;
	}
	
	BasicTransaction::BasicTransaction(BasicTransaction&& rhs) noexcept:
		signer_(this),
		version_(rhs.version_),
		networkId_(rhs.networkId_),
		type_(rhs.type_)
	{
		signer_ = std::move(rhs.signer_);
	}
	
	BasicTransaction::~BasicTransaction()
	{ }
	
	TransactionType BasicTransaction::type() const
	{
		return type_;
	}
	
	uint8_t BasicTransaction::version() const
	{
		return version_;
	}
	
	NetworkIdentifier BasicTransaction::networkId() const
	{
		return networkId_;
	}
	
	const PublicAccount& BasicTransaction::signer() const
	{
		return *signer_;
	}
	
	void BasicTransaction::setSigner(const Key& publicKey)
	{
		signer_.emplace(publicKey, networkId_);
	}
	
	void BasicTransaction::initSigner(std::optional<PublicAccount>& value)
	{
		XPX_CHAIN_SDK_THROW(transaction_error, "signer is not initialized");
	}
}
