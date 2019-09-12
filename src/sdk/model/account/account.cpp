/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/account/account.h>
#include <xpxchaincpp/model/transaction/transaction_signing.h>

namespace xpx_chain_sdk {
	
	Account::Account(PrivateKeySupplier privateKeySupplier, NetworkIdentifier networkId):
		privateKeySupplier_(privateKeySupplier),
		networkId_(networkId)
	{ }
	
	NetworkIdentifier Account::networkId() const
	{
		return networkId_;
	}
	
	PrivateKey Account::privateKey() const
	{
		return requestPrivateKey(PrivateKeySupplierReason::Private_Key_Requested);
	}
	
	KeyPair Account::keyPair() const
	{
		return KeyPair(privateKey());
	}
	
	const Key& Account::publicKey() const
	{
		return publicAccount().publicKey();
	}
	
	const Address& Account::address() const
	{
		return publicAccount().address();
	}
	
	const PublicAccount& Account::publicAccount() const
	{
		if (!publicAccount_) {
			requestPrivateKey(PrivateKeySupplierReason::Public_Account_Init);
		}
		
		return publicAccount_.value();
	}
	
	void Account::signTransaction(Transaction* transaction) const
	{
		if (networkId_ != transaction->networkId()) {
			XPX_CHAIN_SDK_THROW_2(account_error, "account and transaction network ids do not match",
			                 to_underlying_type(networkId_),
			                 to_underlying_type(transaction->networkId()));
		}
		
		KeyPair keyPair(requestPrivateKey(PrivateKeySupplierReason::Transaction_Signing, transaction));
		SignTransaction(transaction, keyPair);
	}
	
	Signature Account::cosignAggregateTransaction(const AggregateTransaction* transaction) const
	{
		if (networkId_ != transaction->networkId()) {
			XPX_CHAIN_SDK_THROW_2(account_error, "account and transaction network ids do not match",
			                 to_underlying_type(networkId_),
			                 to_underlying_type(transaction->networkId()));
		}

		KeyPair keyPair(requestPrivateKey(PrivateKeySupplierReason::Aggregate_Transaction_Cosigning, transaction));
		return CalculateAggregateTransactionCosignature(transaction, keyPair);
	}
	
	PrivateKey Account::requestPrivateKey(PrivateKeySupplierReason reason, PrivateKeySupplierParam param) const
	{
		auto privateKey = privateKeySupplier_(reason, std::move(param));
		
		if (!privateKey) {
			XPX_CHAIN_SDK_THROW(account_error, "failed to obtain private key");
		}
		
		if (!publicAccount_) {
			publicAccount_.emplace(KeyPair::GeneratePublicKey(privateKey.value()), networkId_);
		}
		
		return std::move(privateKey.value());
	}
}
