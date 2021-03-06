/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction/alias_transaction_base.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Mosaic alias transaction base class.
	template<typename TBase>
	class TMosaicAliasTransaction: public TAliasTransactionBase<TBase> {
	public:
		/// Creates mosaic alias transaction.
		template<typename... TArgs>
		explicit TMosaicAliasTransaction(AliasTransactionAction action,
		                                 NamespaceId aliasNamespaceId,
		                                 MosaicId aliasedMosaicId,
		                                 TArgs&&... args):
			TAliasTransactionBase<TBase>(action, aliasNamespaceId, TransactionType::Mosaic_Alias,
			                             std::forward<TArgs>(args)...),
			aliasedMosaicId_(aliasedMosaicId)
		{ }
		
		/// Returns aliased mosaic id.
		MosaicId aliasedMosaicId() const;
		
	private:
		MosaicId aliasedMosaicId_;
	};
	
	extern template class TMosaicAliasTransaction<Transaction>;
	extern template class TMosaicAliasTransaction<EmbeddedTransaction>;
	
	using MosaicAliasTransaction = TMosaicAliasTransaction<Transaction>;
	using EmbeddedMosaicAliasTransaction = TMosaicAliasTransaction<EmbeddedTransaction>;
	
	/// Creates mosaic alias transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<MosaicAliasTransaction>
	CreateMosaicAliasTransaction(AliasTransactionAction action,
	                             NamespaceId aliasNamespaceId,
	                             MosaicId aliasedMosaicId,
	                             std::optional<Amount> maxFee = std::nullopt,
	                             std::optional<NetworkDuration> deadline = std::nullopt,
	                             std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded mosaic alias transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedMosaicAliasTransaction>
	CreateEmbeddedMosaicAliasTransaction(AliasTransactionAction action,
	                                     NamespaceId aliasNamespaceId,
	                                     MosaicId aliasedMosaicId,
	                                     const Key& signer,
	                                     std::optional<NetworkIdentifier> networkId = std::nullopt);
}
