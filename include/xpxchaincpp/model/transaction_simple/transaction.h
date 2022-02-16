/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <xpxchaincpp/types.h>
#include <xpxchaincpp/model/transaction/account_link_transaction_types.h>
#include "xpxchaincpp/model/transaction/account_property_transaction_types.h"
#include "xpxchaincpp/model/transaction/transaction_type.h"
#include "xpxchaincpp/model/transaction/modify_multisig_account_transaction_types.h"
#include "xpxchaincpp/model/transaction/mosaic_supply_change_transaction_types.h"
#include "xpxchaincpp/model/transaction/aggregate_transaction_types.h"
#include "xpxchaincpp/model/transaction/secret_hash_algorithm.h"
#include "xpxchaincpp/model/transaction/alias_transaction_types.h"
#include <xpxchaincpp/model/namespace/namespace.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <memory>

//using EmbeddedTransaction = xpx_chain_sdk::transactions_info::EmbeddedTransaction;
namespace xpx_chain_sdk { namespace transactions_info {

	class BasicTransaction {
	public:
		TransactionType type;
		virtual ~BasicTransaction() = default;
	};

	class Cosignature {
	public:
		std::string publicKey;
		std::string signature;
	};

	class CosignatoryModification {
	public:
		std::string publicKey;
		CosignatoryModificationType modificationType;
	};

    class Transaction : public BasicTransaction {
    public:
        uint32_t size;
        std::string signature;
        std::string signer;
        int32_t version;
        Amount maxFee;
        int64_t deadline;
    };

    class EmbeddedTransaction : public BasicTransaction {
    public:
        uint32_t size;
        std::string signer;
		uint32_t version;
    };

	class TransactionContainer {
	public:
		std::vector<std::shared_ptr<BasicTransaction> > transactions;
	public:
		void Add(const std::shared_ptr<BasicTransaction>& transaction) {
			transactions.push_back(transaction);
		}

	};

    class AggregateTransaction : public Transaction {
    public:
        uint32_t payloadSize;
        TransactionContainer transactions;
        std::vector<Cosignature> cosignatures;
    };

    template<typename TBase>
    class TAccountLinkTransaction : public TBase {
    public:
        AccountLinkTransactionAction linkAction;
        std::string remoteAccountKey;
    };

    template<typename TBase>
    class TLockFundsTransaction : public TBase {
    public:
        Mosaic lockedMosaic;
        BlockDuration lockDuration;
        std::string  lockHash;
    };

    template<typename TBase>
    class TModifyMultisigAccountTransaction : public TBase {
    public:
        int8_t minRemovalDelta;
        int8_t minApprovalDelta;
        uint8_t modificationsCount;
        std::vector<CosignatoryModification> modifications; // CosignatoryModifications is already container
    };

    template<typename TBase>
    class TMosaicDefinitionTransaction : public TBase {
    public:
        uint32_t nonce;
        MosaicId mosaicId;
        uint8_t optionalPropertiesCount;
        MosaicFlags flags;
        uint8_t divisibility;
        std::vector<MosaicProperty> optionalProperties;
    };

    template<typename TBase>
    class TMosaicSupplyChangeTransaction: public TBase {
    public:
        MosaicId mosaicId;
        MosaicSupplyChangeDirection direction;
        Amount delta;
    };

    template<typename TBase>
    class TRegisterNamespaceTransaction: public TBase {
    public:
        NamespaceType namespaceType;
        uint64_t durationOrParentId;
        NamespaceId namespaceId;
        uint8_t namespaceNameSize;
        std::string namespaceName;
    };

    template<typename TBase>
    class TSecretLockTransaction: public TBase {
    public:
        MosaicId mosaicId;
        Amount amount;
        BlockDuration duration;
        SecretHashAlgorithm hashAlgorithm;
        std::string secret;
        std::string recipient;
    };

    template<typename TBase>
    class TSecretProofTransaction: public TBase {
    public:
        SecretHashAlgorithm hashAlgorithm;
        std::string  secret;
        uint16_t proofSize;
        std::vector<uint8_t> proof;
    };

    template<typename TBase>
    class TStoragePaymentTransaction: public TBase {
    public:
        std::string driveKey;
        Amount storageUnits;
    };

    class TransferTransactionMessage {
    public:
        std::uint32_t type;
        std::string payload;
    };

    template<typename TBase>
    class TTransferTransaction: public TBase {
    public:
        std::string recipient;
        uint16_t messageSize;
        uint8_t mosaicsCount;
        TransferTransactionMessage message;
        std::vector<Mosaic> mosaics;
    };

    template<typename TBase>
    class TAliasTransactionBase: public TBase {

    public:
        AliasTransactionAction aliasAction;
        NamespaceId namespaceId;
    };

    template<typename TBase>
    class TAddressAliasTransaction: public TAliasTransactionBase<TBase> {
    public:
        std::string address;
    };

    template<typename TBase>
    class TMosaicAliasTransaction: public TAliasTransactionBase<TBase> {
    public:
        MosaicId mosaicId;
    };

    template<typename TBase, typename T>
    class  TAccountPropertyTransaction: public TBase {
    public:
        uint8_t propertyType;
        uint8_t modificationsCount;
        std::vector<AccountPropertyModification<T>> modifications;
    };

    template<typename TBase>
    class TPrepareBcDriveTransaction: public TBase {
    public:
		uint64_t driveSize;
		Amount verificationFeeAmount;
        uint16_t replicatorCount;

    };

    template<typename TBase>
    class TDataModificationTransaction: public TBase {
    public:
		std::string driveKey;
		std::string downloadDataCdi;
		uint64_t uploadSize;
        Amount feedbackFeeAmount;

    };

    template<typename TBase>
    class TDownloadTransaction: public TBase {
    public:
		std::string driveKey;
		uint64_t downloadSize;
		Amount feedbackFeeAmount;
        uint16_t listOfPublicKeysSize;
        std::vector<std::string> listOfPublicKeys;

    };

    template<typename TBase>
    class TDownloadPaymentTransaction: public TBase {
    public:
        std::string downloadChannelId;
        uint64_t downloadSize;
        Amount feedbackFeeAmount;

    };

    template<typename TBase>
    class TDriveClosureTransaction: public TBase {
    public:
        std::string driveKey;

    };

    template<typename TBase>
    class TDataModificationApprovalTransaction: public TBase {
    public:
        std::string driveKey;
        std::string dataModificationId;
        std::string fileStructureCdi;
        uint64_t fileStructureSize;
        uint64_t metaFilesSize;
        uint64_t usedDriveSize;
        uint8_t judgingKeysCount;
        uint8_t overlappingKeysCount;
        uint8_t judgedKeysCount;
        uint16_t opinionElementCount;
        std::vector<std::string> publicKeys;
        std::vector<std::string> signatures;
        std::vector<uint8_t> presentOpinions;
        std::vector<uint64_t> opinions;

    };

    template<typename TBase>
    class TDataModificationCancelTransaction: public TBase {
    public:
		std::string driveKey;
		std::string dataModificationId;

    };

    template<typename TBase>
    class TFinishDownloadTransaction: public TBase {
    public:
        std::string downloadChannelId;
        Amount feedbackFeeAmount;

    };

    template<typename TBase>
    class TReplicatorOnboardingTransaction: public TBase {
    public:
		Amount capacity;

    };

    template<typename TBase>
    class TReplicatorOffboardingTransaction: public TBase {
    public:
        std::string driveKey;

    };

    using AccountLinkTransaction  = TAccountLinkTransaction<Transaction>;
    using EmbeddedAccountLinkTransaction  = TAccountLinkTransaction<EmbeddedTransaction>;

    using LockFundsTransaction  = TLockFundsTransaction<Transaction>;
    using EmbeddedLockFundsTransaction  = TLockFundsTransaction<EmbeddedTransaction>;

    using ModifyMultisigAccountTransaction  = TModifyMultisigAccountTransaction<Transaction >;
    using EmbeddedModifyMultisigAccountTransaction  = TModifyMultisigAccountTransaction<EmbeddedTransaction >;

    using MosaicDefinitionTransaction  = TMosaicDefinitionTransaction <Transaction>;
    using EmbeddedMosaicDefinitionTransaction  = TMosaicDefinitionTransaction<EmbeddedTransaction>;

    using MosaicSupplyChangeTransaction  = TMosaicSupplyChangeTransaction<Transaction>;
    using EmbeddedMosaicSupplyChangeTransaction  = TMosaicSupplyChangeTransaction<EmbeddedTransaction >;

    using RegisterNamespaceTransaction  = TRegisterNamespaceTransaction<Transaction >;
    using EmbeddedRegisterNamespaceTransaction  = TRegisterNamespaceTransaction<EmbeddedTransaction >;

    using SecretLockTransaction  = TSecretLockTransaction<Transaction >;
    using EmbeddedSecretLockTransaction  = TSecretLockTransaction<EmbeddedTransaction >;

    using SecretProofTransaction  = TSecretProofTransaction<Transaction >;
    using EmbeddedSecretProofTransaction  = TSecretProofTransaction<EmbeddedTransaction >;

    using StoragePaymentTransaction  = TStoragePaymentTransaction<Transaction >;
    using EmbeddedStoragePaymentTransaction  = TStoragePaymentTransaction<EmbeddedTransaction >;

    using TransferTransaction  = TTransferTransaction <Transaction>;
    using EmbeddedTransferTransaction  = TTransferTransaction<EmbeddedTransaction >;

    using AddressAliasTransaction  = TAddressAliasTransaction<Transaction >;
    using EmbeddedAddressAliasTransaction  = TAddressAliasTransaction<EmbeddedTransaction >;

    using MosaicAliasTransaction  = TMosaicAliasTransaction<Transaction >;
    using EmbeddedMosaicAliasTransaction  = TMosaicAliasTransaction<EmbeddedTransaction >;

    using AbstractAccountPropertyTransaction = TAccountPropertyTransaction<Transaction, std::string>;
    using EmbededAbastractAccountPropertyTransaction = TAccountPropertyTransaction<EmbeddedTransaction, std::string>;

    using AccountAddressPropertyTransaction  = TAccountPropertyTransaction<Transaction, std::string>;
    using EmbeddedAccountAddressPropertyTransaction  = TAccountPropertyTransaction<EmbeddedTransaction, std::string>;

    using AccountMosaicPropertyTransaction  = TAccountPropertyTransaction<Transaction, MosaicId>;
    using EmbeddedAccountMosaicPropertyTransaction  = TAccountPropertyTransaction <EmbeddedTransaction, MosaicId>;

    using AccountTransactionPropertyTransaction  = TAccountPropertyTransaction<Transaction, TransactionType>;
    using EmbeddedAccountTransactionPropertyTransaction  = TAccountPropertyTransaction<EmbeddedTransaction, TransactionType>;

    using AliasTransactionBase = TAliasTransactionBase<Transaction>;
    using EmbeddedAliasTransactionBase = TAliasTransactionBase<EmbeddedTransaction>;

    using PrepareBcDriveTransaction = TPrepareBcDriveTransaction <Transaction>;
    using EmbeddedPrepareBcDriveTransaction = TPrepareBcDriveTransaction<EmbeddedTransaction>;

    using DataModificationTransaction = TDataModificationTransaction <Transaction>;
    using EmbeddedDataModificationTransaction = TDataModificationTransaction<EmbeddedTransaction>;

    using DownloadTransaction = TDownloadTransaction <Transaction>;
    using EmbeddedDownloadTransaction = TDownloadTransaction<EmbeddedTransaction>;

    using DownloadPaymentTransaction = TDownloadPaymentTransaction <Transaction>;
    using EmbeddedDownloadPaymentTransaction = TDownloadPaymentTransaction<EmbeddedTransaction>;

    using DriveClosureTransaction = TDriveClosureTransaction <Transaction>;
    using EmbeddedDriveClosureTransaction = TDriveClosureTransaction<EmbeddedTransaction>;

    using DataModificationApprovalTransaction = TDataModificationApprovalTransaction <Transaction>;
    using EmbeddedDataModificationApprovalTransaction = TDataModificationApprovalTransaction<EmbeddedTransaction>;

    using DataModificationCancelTransaction = TDataModificationCancelTransaction <Transaction>;
    using EmbeddedDataModificationCancelTransaction = TDataModificationCancelTransaction<EmbeddedTransaction>;

    using FinishDownloadTransaction = TFinishDownloadTransaction <Transaction>;
    using EmbeddedFinishDownloadTransaction = TFinishDownloadTransaction<EmbeddedTransaction>;

    using ReplicatorOnboardingTransaction = TReplicatorOnboardingTransaction <Transaction>;
    using EmbeddedReplicatorOnboardingTransaction = TReplicatorOnboardingTransaction<EmbeddedTransaction>;

    using ReplicatorOffboardingTransaction = TReplicatorOffboardingTransaction <Transaction>;
    using EmbeddedReplicatorOffboardingTransaction = TReplicatorOffboardingTransaction<EmbeddedTransaction>;
}}





