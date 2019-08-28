
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/config.h>

#include "sdk/model/transaction/transaction_utils.h"

namespace xpx_sdk {
	
	Transaction::Transaction(TransactionType type,
	                         uint32_t fullVersion,
	                         Amount maxFee,
	                         int64_t deadline,
	                         std::vector<uint8_t> binaryData,
	                         const std::optional<Key>& signer,
	                         const std::optional<Signature>& signature,
	                         const std::optional<TransactionInfo>& info):
		BasicTransaction(type, fullVersion, signer),
		signature_(this, signature),
		maxFee_(maxFee),
		deadline_(NetworkDuration(deadline)),
		hash_(this),
		binary_(std::move(binaryData)),
		info_{}
	{
		if (info) {
			info_ = *info;
		}
	}
	
	Transaction::Transaction(const Transaction& rhs):
		BasicTransaction(rhs),
		signature_(this),
		maxFee_(rhs.maxFee_),
		deadline_(rhs.deadline_),
		hash_(this),
		binary_(rhs.binary_),
		info_(rhs.info_)
	{
		signature_ = rhs.signature_;
		hash_ = rhs.hash_;
	}
	
	Transaction::Transaction(Transaction&& rhs) noexcept:
		BasicTransaction(std::move(rhs)),
		signature_(this),
		maxFee_(rhs.maxFee_),
		deadline_(rhs.deadline_),
		hash_(this),
		binary_(rhs.binary_),
		info_(rhs.info_)
	{
		signature_ = std::move(rhs.signature_);
		hash_ = std::move(rhs.hash_);
	}
	
	bool Transaction::isEmbedded() const
	{
		return false;
	}
	
	bool Transaction::isSigned() const
	{
		return static_cast<bool>(signature_);
	}
	
	Amount Transaction::maxFee() const
	{
		return maxFee_;
	}
	
	NetworkTimepoint Transaction::deadline() const
	{
		return deadline_;
	}
	
	const Signature& Transaction::signature() const
	{
		return *signature_;
	}
	
	const Hash256& Transaction::hash() const
	{
		return *hash_;
	}
	
	const std::vector<uint8_t>& Transaction::binary() const
	{
		return binary_;
	}
	
	const TransactionInfo& Transaction::info() const
	{
		return info_;
	}
	
	Amount Transaction::GetDefaultFee(size_t transactionSize)
	{
		const auto& config = GetConfig();
		return static_cast<Amount>(transactionSize * config.BlockAvgFeeMultiplier * config.TransactionFeeMultiplier);
	}
	
	NetworkDuration Transaction::GetDefaultDeadline()
	{

		auto now = std::chrono::system_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - GetConfig().NetworkEpoch);
		return diff + GetConfig().TransactionDelta;
	}
	
	void Transaction::initSignature(std::optional<Signature>&)
	{
		NEM2_SDK_THROW(transaction_error, "signature is not initialized");
	}
	
	void Transaction::initHash(std::optional<Hash256>& value)
	{
		value = internal::CalculateTransactionHash(this);
	}
}
