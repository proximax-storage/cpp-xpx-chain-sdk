/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Manual call transaction base class.
	template<typename TBase>
    class TManualCallTransaction: public TBase {
	public:
		/// Creates Manual call transaction.
		template<typename... TArgs>
		explicit TManualCallTransaction(
                const Key& contractKey,
                const std::string fileName,
                const std::string functionName,
                const std::vector<uint8_t>& actualArguments,
                const Amount& executionCallPayment,
                const Amount& downloadCallPayment,
                MosaicContainer servicePayments,
		        TArgs&&... args):
            TBase(TransactionType::Manual_Call, std::forward<TArgs>(args)...),
            contractKey_(contractKey),
            fileName_(fileName),
            functionName_(functionName),
            actualArguments_(actualArguments),
            executionCallPayment_(executionCallPayment),
            downloadCallPayment_(downloadCallPayment),
            servicePayments_(servicePayments)
		{ }

        const Key& contractKey() const;

        const std::string& fileName() const;

        const std::string& functionName() const;

        const std::vector<uint8_t>& actualArguments() const;

        const Amount& executionCallPayment() const;

        const Amount& downloadCallPayment() const;

        const MosaicContainer& servicePayments() const;

	private:
        Key contractKey_;
        std::string fileName_;
        std::string functionName_;
        std::vector<uint8_t> actualArguments_;
        Amount executionCallPayment_;
        Amount downloadCallPayment_;
        MosaicContainer servicePayments_;
	};
	
	extern template class TManualCallTransaction<Transaction>;
	extern template class TManualCallTransaction<EmbeddedTransaction>;
	
	using ManualCallTransaction = TManualCallTransaction<Transaction>;
	using EmbeddedManualCallTransaction = TManualCallTransaction<EmbeddedTransaction>;
	
	/// Creates Manual call transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<ManualCallTransaction>
	CreateManualCallTransaction(const Key& contractKey,
                            const std::string& fileName,
                            const std::string& functionName,
                            const std::vector<uint8_t>& actualArguments,
                            const Amount& executionCallPayment,
                            const Amount& downloadCallPayment,
                            MosaicContainer servicePayments,
                            std::optional<Amount> maxFee = std::nullopt,
                            std::optional<NetworkDuration> deadline = std::nullopt,
                            std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	
	/// Creates Manual call transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedManualCallTransaction>
	CreateEmbeddedManualCallTransaction(const Key& contractKey,
                                    const std::string& fileName,
                                    const std::string& functionName,
                                    const std::vector<uint8_t>& actualArguments,
                                    const Amount& executionCallPayment,
                                    const Amount& downloadCallPayment,
                                    MosaicContainer servicePayments,
                                    const Key& signer,
                                    std::optional<NetworkIdentifier> networkId = std::nullopt);
}
