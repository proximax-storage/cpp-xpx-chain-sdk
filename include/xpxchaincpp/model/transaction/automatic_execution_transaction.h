/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Automatic executions payment transaction base class.
	template<typename TBase>
    class TAutomaticExecutionsPaymentTransaction: public TBase {
	public:
		/// Creates Automatic executions payment transaction.
		template<typename... TArgs>
		explicit TAutomaticExecutionsPaymentTransaction(
                const Key& contractKey,
                uint32_t automaticExecutionsNumber,
		        TArgs&&... args):
            TBase(TransactionType::Automatic_Executions_Payment, std::forward<TArgs>(args)...),
            contractKey_(contractKey),
            automaticExecutionsNumber_(automaticExecutionsNumber)
		{ }

        const Key& contractKey() const;

        uint32_t automaticExecutionsNumber() const;
		
	private:
        Key contractKey_;
        uint32_t automaticExecutionsNumber_;
	};
	
	extern template class TAutomaticExecutionsPaymentTransaction<Transaction>;
	extern template class TAutomaticExecutionsPaymentTransaction<EmbeddedTransaction>;
	
	using AutomaticExecutionsPaymentTransaction = TAutomaticExecutionsPaymentTransaction<Transaction>;
	using EmbeddedAutomaticExecutionsPaymentTransaction = TAutomaticExecutionsPaymentTransaction<EmbeddedTransaction>;
	
	/// Creates Automatic executions payment transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AutomaticExecutionsPaymentTransaction>
	CreateAutomaticExecutionsPaymentTransaction(const Key& contractKey,
                                                uint32_t automaticExecutionsNumber,
                                                std::optional<Amount> maxFee = std::nullopt,
                                                std::optional<NetworkDuration> deadline = std::nullopt,
                                                std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	
	/// Creates Automatic executions payment transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedAutomaticExecutionsPaymentTransaction>
	CreateEmbeddedAutomaticExecutionsPaymentTransaction(const Key& contractKey,
                                                        uint32_t automaticExecutionsNumber,
                                                        const Key& signer,
                                                        std::optional<NetworkIdentifier> networkId = std::nullopt);
}
