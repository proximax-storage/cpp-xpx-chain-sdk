
#pragma once

#include <nemcpp/model/account/public_account.h>
#include <nemcpp/model/transaction/account_link_transaction_types.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_sdk::complex_transactions {
	
	/// Account link transaction base class.
	/// \note Delegates harvesting to a remote account.
	template<typename TBase>
	class TAccountLinkTransaction: public TBase {
	public:
		/// Creates account link transaction.
		template<typename... TArgs>
		explicit TAccountLinkTransaction(AccountLinkTransactionAction action,
		                                 const Key& remoteAccountKey,
		                                 TArgs&&... args):
			TBase(TransactionType::Account_Link, std::forward<TArgs>(args)...),
			action_(action),
			remotePublicAccount_(remoteAccountKey, TBase::networkId())
		{ }
		
		/// Returns account link transaction action.
		AccountLinkTransactionAction action() const;
		
		/// Returns remote public account.
		const PublicAccount& remotePublicAccount() const;
		
	private:
		AccountLinkTransactionAction action_;
		PublicAccount remotePublicAccount_;
	};
	
	extern template class TAccountLinkTransaction<Transaction>;
	extern template class TAccountLinkTransaction<EmbeddedTransaction>;
	
	using AccountLinkTransaction = TAccountLinkTransaction<Transaction>;
	using EmbeddedAccountLinkTransaction = TAccountLinkTransaction<EmbeddedTransaction>;
	
	/// Creates account link transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AccountLinkTransaction>
	CreateAccountLinkTransaction(AccountLinkTransactionAction action,
	                             const Key& remoteAccountKey,
	                             std::optional<Amount> maxFee = std::nullopt,
	                             std::optional<NetworkDuration> deadline = std::nullopt,
	                             std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded account link transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedAccountLinkTransaction>
	CreateEmbeddedAccountLinkTransaction(AccountLinkTransactionAction action,
	                                     const Key& remoteAccountKey,
	                                     const Key& signer,
	                                     std::optional<NetworkIdentifier> networkId = std::nullopt);
}
