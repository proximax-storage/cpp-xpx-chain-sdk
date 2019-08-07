
#include <nemcpp/model/transaction/basic_transaction.h>
#include <nemcpp/config.h>

#include "sdk/model/transaction/transaction_utils.h"

namespace xpx_sdk::difficult_transactions {
	
	BasicTransaction::BasicTransaction(TransactionType type, uint16_t fullVersion, const std::optional<Key>& signer):
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
		NEM2_SDK_THROW(transaction_error, "signer is not initialized");
	}
}
