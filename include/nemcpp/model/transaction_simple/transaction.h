#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <nemcpp/types.h>
#include "account_link_transaction_types.h"
#include "account_property_transaction_types.h"
#include "transaction_type.h"
#include "modify_multisig_account_transaction_types.h"
#include "mosaic_supply_change_transaction_types.h"
#include "aggregate_transaction_types.h"
#include "secret_hash_algorithm.h"
#include "alias_transaction_types.h"
#include "transaction_container.h"
#include <nemcpp/model/namespace/namespace.h>
#include <nemcpp/model/mosaic/mosaic.h>
#include <memory>


namespace xpx_sdk { namespace simple_transactions {

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
        int64_t version;
        TransactionType type;
        Amount maxFee;
        int64_t deadline;
    };

    class EmbeddedTransaction : public BasicTransaction {
    public:
        uint32_t size;
        std::string signer;
        uint16_t version;
        TransactionType type;
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
    class TTransferTransaction: public TBase {
    public:
        std::string recipient;
        uint16_t messageSize;
        uint8_t mosaicsCount;
        std::vector<uint8_t> message;
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

    using TransferTransaction  = TTransferTransaction <Transaction >;
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
}}





