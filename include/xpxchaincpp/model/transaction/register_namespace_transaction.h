/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/namespace/namespace.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Namespace registration transaction base class.
	template<typename TBase>
	class TRegisterNamespaceTransaction: public TBase {
	public:
		/// Creates namespace registration transaction.
		template<typename... TArgs>
		explicit TRegisterNamespaceTransaction(Namespace ns,
		                                       uint64_t nsDuration,
		                                       TArgs&&... args):
			TBase(TransactionType::Register_Namespace, std::forward<TArgs>(args)...),
			namespace_(std::move(ns)),
			namespaceDuration_(nsDuration)
		{ }
		
		/// Returns namespace.
		const Namespace& ns() const;
		
		/// Returns namespace duration.
		/// \note For child namespaces returns 0.
		BlockDuration nsDuration() const;
		
	private:
		Namespace namespace_;
		BlockDuration namespaceDuration_;
	};
	
	extern template class TRegisterNamespaceTransaction<Transaction>;
	extern template class TRegisterNamespaceTransaction<EmbeddedTransaction>;
	
	using RegisterNamespaceTransaction = TRegisterNamespaceTransaction<Transaction>;
	using EmbeddedRegisterNamespaceTransaction = TRegisterNamespaceTransaction<EmbeddedTransaction>;
	
	/// Creates root namespace registration transaction.
	/// \note Throws \c transaction_error if \a namespaceName size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterRootNamespaceTransaction(std::string_view namespaceName,
	                                       BlockDuration namespaceDuration,
	                                       std::optional<Amount> maxFee = std::nullopt,
	                                       std::optional<NetworkDuration> deadline = std::nullopt,
	                                       std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates child namespace registration transaction.
	/// \note Throws \c transaction_error if \a namespaceName size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterChildNamespaceTransaction(std::string_view namespaceName,
	                                        NamespaceId namespaceParentId,
	                                        std::optional<Amount> maxFee = std::nullopt,
	                                        std::optional<NetworkDuration> deadline = std::nullopt,
	                                        std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded root namespace registration transaction.
	/// \note Throws \c transaction_error if \a namespaceName size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedRegisterNamespaceTransaction>
	CreateEmbeddedRegisterRootNamespaceTransaction(std::string_view namespaceName,
	                                               BlockDuration namespaceDuration,
	                                               const Key& signer,
	                                               std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Create embedded child namespace registration transaction.
	/// \note Throws \c transaction_error if \a namespaceName size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedRegisterNamespaceTransaction>
	CreateEmbeddedRegisterChildNamespaceTransaction(std::string_view namespaceName,
	                                                NamespaceId namespaceParentId,
	                                                const Key& signer,
	                                                std::optional<NetworkIdentifier> networkId = std::nullopt);
}
