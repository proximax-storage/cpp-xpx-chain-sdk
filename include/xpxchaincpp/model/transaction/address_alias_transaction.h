/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/account/address.h>
#include <xpxchaincpp/model/transaction/alias_transaction_base.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Address alias transaction base class.
	template<typename TBase>
	class TAddressAliasTransaction: public TAliasTransactionBase<TBase> {
	public:
		/// Creates address alias transaction.
		template<typename... TArgs>
		explicit TAddressAliasTransaction(AliasTransactionAction action,
		                                  NamespaceId aliasNamespaceId,
		                                  const Address& aliasedAddress,
		                                  TArgs&&... args):
			TAliasTransactionBase<TBase>(action, aliasNamespaceId, TransactionType::Address_Alias,
			                             std::forward<TArgs>(args)...),
			aliasedAddress_(aliasedAddress)
		{ }
		
		/// Returns aliased address.
		const Address& aliasedAddress() const;
		
	private:
		Address aliasedAddress_;
	};
	
	extern template class TAddressAliasTransaction<Transaction>;
	extern template class TAddressAliasTransaction<EmbeddedTransaction>;
	
	using AddressAliasTransaction = TAddressAliasTransaction<Transaction>;
	using EmbeddedAddressAliasTransaction = TAddressAliasTransaction<EmbeddedTransaction>;
	
	/// Creates address alias transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AddressAliasTransaction>
	CreateAddressAliasTransaction(AliasTransactionAction action,
	                              NamespaceId aliasNamespaceId,
	                              const Address& aliasedAddress,
	                              std::optional<Amount> maxFee = std::nullopt,
	                              std::optional<NetworkDuration> deadline = std::nullopt,
	                              std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded address alias transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedAddressAliasTransaction>
	CreateEmbeddedAddressAliasTransaction(AliasTransactionAction action,
	                                      NamespaceId aliasNamespaceId,
	                                      const Address& aliasedAddress,
	                                      const Key& signer,
	                                      std::optional<NetworkIdentifier> networkId = std::nullopt);
}
