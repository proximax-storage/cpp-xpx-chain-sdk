/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/buffer.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_chain_sdk {

	/// Deploy contract transaction base class.
	template<typename TBase>
    class TDeployContractTransaction: public TBase {
	public:
		/// Creates Deploy contract transaction.
		template<typename... TArgs>
		explicit TDeployContractTransaction(
                const Key& driveKey,
                const std::string fileName,
                const std::string functionName,
                const std::vector<uint8_t>& actualArguments,
                const Amount& executionCallPayment,
                const Amount& downloadCallPayment,
                const std::vector<MosaicId>& servicePayments,
                const std::string automaticExecutionsFileName,
                const std::string automaticExecutionsFunctionName,
                const Amount& automaticExecutionsCallPayment,
                const Amount& automaticDownloadCallPayment,
                uint32_t automaticExecutionsNumber,
                const Key& assignee,
		        TArgs&&... args):
            TBase(TransactionType::Deploy_Contract, std::forward<TArgs>(args)...),
            driveKey_(driveKey),
            fileName_(fileName),
            functionName_(functionName),
            actualArguments_(actualArguments),
            executionCallPayment_(executionCallPayment),
            downloadCallPayment_(downloadCallPayment),
            servicePayments_(servicePayments),
            automaticExecutionsFileName_(automaticExecutionsFileName),
            automaticExecutionsFunctionName_(automaticExecutionsFunctionName),
            automaticExecutionsCallPayment_(automaticExecutionsCallPayment),
            automaticDownloadCallPayment_(automaticDownloadCallPayment),
            automaticExecutionsNumber_(automaticExecutionsNumber),
            assignee_(assignee)
		{ }

        const Key& driveKey() const;

        const std::string& fileName() const;

        const std::string& functionName() const;

        const std::vector<uint8_t>& actualArguments() const;

        const Amount& executionCallPayment() const;

        const Amount& downloadCallPayment() const;

        const std::vector<MosaicId>& servicePayments() const;

        const std::string& automaticExecutionsFileName() const;

        const std::string& automaticExecutionsFunctionName() const;

        const Amount& automaticExecutionsCallPayment() const;

        const Amount& automaticDownloadCallPayment() const;

        uint32_t automaticExecutionsNumber() const;

        const Key& assignee() const;

    private:
        Key driveKey_;
        std::string fileName_;
        std::string functionName_;
        std::vector<uint8_t> actualArguments_;
        Amount executionCallPayment_;
        Amount downloadCallPayment_;
        std::vector<MosaicId> servicePayments_;
        std::string automaticExecutionsFileName_;
        std::string automaticExecutionsFunctionName_;
        Amount automaticExecutionsCallPayment_;
        Amount automaticDownloadCallPayment_;
        uint32_t automaticExecutionsNumber_;
        Key assignee_;
	};

	extern template class TDeployContractTransaction<Transaction>;
	extern template class TDeployContractTransaction<EmbeddedTransaction>;

	using DeployContractTransaction = TDeployContractTransaction<Transaction>;
	using EmbeddedDeployContractTransaction = TDeployContractTransaction<EmbeddedTransaction>;

	/// Creates Deploy contract transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DeployContractTransaction>
	CreateDeployContractTransaction(const Key& driveKey,
                                    const std::string& fileName,
                                    const std::string& functionName,
                                    const std::vector<uint8_t>& actualArguments,
                                    const Amount& executionCallPayment,
                                    const Amount& downloadCallPayment,
                                    const std::vector<MosaicId>& servicePayments,
                                    const std::string& automaticExecutionsFileName,
                                    const std::string& automaticExecutionsFunctionName,
                                    const Amount& automaticExecutionsCallPayment,
                                    const Amount& automaticDownloadCallPayment,
                                    uint32_t automaticExecutionsNumber,
                                    const Key& assignee,
                                    std::optional<Amount> maxFee = std::nullopt,
                                    std::optional<NetworkDuration> deadline = std::nullopt,
                                    std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates deploy contract transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedDeployContractTransaction>
	CreateEmbeddedDeployContractTransaction(const Key& driveKey,
                                            const std::string& fileName,
                                            const std::string& functionName,
                                            const std::vector<uint8_t>& actualArguments,
                                            const Amount& executionCallPayment,
                                            const Amount& downloadCallPayment,
                                            const std::vector<MosaicId>& servicePayments,
                                            const std::string& automaticExecutionsFileName,
                                            const std::string& automaticExecutionsFunctionName,
                                            const Amount& automaticExecutionsCallPayment,
                                            const Amount& automaticDownloadCallPayment,
                                            uint32_t automaticExecutionsNumber,
                                            const Key& assignee,
                                            const Key& signer,
											std::optional<NetworkIdentifier> networkId = std::nullopt);
}
