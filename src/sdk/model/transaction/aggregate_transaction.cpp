
#include <nemcpp/model/transaction/aggregate_transaction.h>

namespace xpx_sdk {
	
	bool AggregateTransaction::isComplete() const
	{
		return type() == TransactionType::Aggregate_Complete;
	}
	
	const EmbeddedTransactions& AggregateTransaction::embeddedTransactions() const
	{
		return embeddedTransactions_;
	}
	
	const std::unordered_set<PublicAccount>& AggregateTransaction::cosigners() const
	{
		return cosigners_;
	}
	
	const Cosignatures& AggregateTransaction::cosignatures() const
	{
		if (isSigned() && !cosignatures_.count(signer())) {
			cosignatures_[signer()] = signature();
		}
		
		return cosignatures_;
	}
	
	size_t AggregateTransaction::payloadSize() const
	{
		return payloadSize_;
	}
}
