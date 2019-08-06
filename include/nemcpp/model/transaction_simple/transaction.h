#include <string>
#include <vector>
#include <stdint.h>
#include <nemcpp/types.h>
#include <nemcpp/model/transaction/transaction_type.h>

namespace xpx_sdk {
    class Transaction {
    public:
        uint32_t size;
        Signature signature;
        Key signer;
        uint16_t version;
        TransactionType type;
        Amount maxFee;
        int64_t deadline;
    };

    Transaction fromDto<Transaction, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        Transaction transaction;
        transaction.size = dto.value<"size"_>();
        transaction.signature = dto.value<"signature"_>();
        transaction.signer = dto.value<"signer"_>();
        transaction.version = dto.value<"version"_>();
        transaction.type = dto.value<"type"_>();
        transaction.maxFee = dto.value<"maxFee"_>();
        transaction.deadline = dto.value<"deadline"_>();
        return transaction;
    }

    class EmbededTransaction {
    public:
        uint32_t size;
        Key signer;
        uint16_t version;
        TransactionType type;
    };

    EmbededTransaction fromDto<EmbededTransaction, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        EmbededTransaction transaction;
        transaction.size = dto.value<"size"_>();
        transaction.signer = dto.value<"signer"_>();
        transaction.version = dto.value<"version"_>();
        transaction.type = dto.value<"type"_>();
        return transaction;
    }

    class AggregateTransaction : public Transaction {
    public:
        AggregateTransaction(const Transaction& other) : Transaction(other) {}
    public:
        uint32_t payloadSize;
        std::vector<uint8_t> payload;
        std::vector<Cosignature > cosignatures;
    };

    AggregateTransaction fromDto<AggregateTransaction, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        AggregateTransaction transaction(fromDto<Transaction, GeneralTransactionDto>(dto));
        transaction.payloadSize = dto.value<"payloadSize"_>();
        transaction.payload = dto.value<"payload"_>();
        transaction.cosignatures = dto.value<"cosignatures"_>();
        return transaction;
    }

    template<typename TBase>
    class TAccountLinkTransaction : TBase {
    public:
        TAccountLinkTransaction(const TBase& other) : TBase(other) {}
    public:

        AccountLinkTransactionAction linkAction;
        Key remoteAccountKey;
    };

    template<typename TBase>
    TAccountLinkTransaction<TBase> fromDto<TAccountLinkTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TAccountLinkTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
        transaction.linkAction = dto.value<"linkAction"_>();
        transaction.remoteAccountKey = dto.value<"remoteAccountKey"_>();
        return transaction;
    }

    template<typename TBase>
    class TLockFundsTransaction : public TBase {
    public:
        TLockFundsTransaction(const TBase& other) : TBase(other) {}
    public:
        Mosaic lockedMosaic;
        BlockDuration lockDuration;
        Hash256 lockHash;
    };

    template<typename TBase>
    TLockFundsTransaction<TBase> fromDto<TLockFundsTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TAccountLinkTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
        transaction.lockedMosaic = dto.value<"lockedMosaic"_>();
        transaction.lockDuration = dto.value<"lockDuration"_>();
        transaction.lockHash = dto.value<"lockHash"_>();
        return transaction;
    }

    template<typename TBase>
    class TModifyMultisigAccountTransaction : public TBase {
    public:
        TModifyMultisigAccountTransaction(const TBase& other) : TBase(other) {}
    public:
        int8_t minRemovalDelta;
        int8_t minApprovalDelta;
        uint8_t modificationsCount;
        std::vector<CosignatoryModification> modifications;
    };

    template<typename TBase>
    TModifyMultisigAccountTransaction<TBase> fromDto<TModifyMultisigAccountTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TModifyMultisigAccountTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
        transaction.minRemovalDelta = dto.value<"minRemovalDelta"_>();
        transaction.minApprovalDelta = dto.value<"minApprovalDelta"_>();
        transaction.modificationsCount = dto.value<"modificationsCount"_>();
        transaction.modifications = dto.value<"modifications"_>();
        return transaction;
    }

    template<typename TBase>
    class TMosaicDefinitionTransaction : public TBase {
    public:
        TMosaicDefinitionTransaction(const TBase& other) : TBase(other) {}
    public:
        uint32_t nonce;
        MosaicId mosaicId;
        uint8_t optionalPropertiesCount;
        MosaicFlags flags;
        uint8_t divisibility;
        std::vector<MosaicProperty> optionalProperties;

    };

    template<typename TBase>
    TMosaicDefinitionTransaction<TBase> fromDto<TMosaicDefinitionTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TMosaicDefinitionTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));

        transaction.nonce = dto.value<"nonce"_>();
        transaction.mosaicId = dto.value<"mosaicId"_>();
        transaction.optionalPropertiesCount = dto.value<"optionalPropertiesCount"_>();
        transaction.flags = dto.value<"flags"_>();
        transaction.divisibility = dto.value<"divisibility"_>();
        for(auto & mosaicProperty : dto.value<"optionalProperties"_>()) {
			transaction.optionalProperties.push_back(fromDto<MosaicProperty, MosaicPropertyDto>(mosaicProperty)) ;
		}
        return transaction;
    }

    template<typename TBase>
    class TMosaicSupplyChangeTransaction: public TBase {
    public:
        TMosaicSupplyChangeTransaction(const TBase& other) : TBase(other) {}
    public:
        MosaicId mosaicId;
        MosaicSupplyChangeDirection direction;
        Amount delta;
    };

    template<typename TBase>
    TMosaicSupplyChangeTransaction<TBase> fromDto<TMosaicSupplyChangeTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TMosaicSupplyChangeTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
        transaction.mosaicId = dto.value<"mosaicId"_>();
        transaction.direction = dto.value<"direction"_>();
        transaction.delta = dto.value<"delta"_>();
        return transaction;
    }

    template<typename TBase>
    class TRegisterNamespaceTransaction: public TBase {
    public:
        TRegisterNamespaceTransaction(const TBase& other) : TBase(other) {}
    public:
        NamespaceType namespaceType;
        uint64_t durationOrParentId;
        NamespaceId namespaceId;
        uint8_t namespaceNameSize;
        std::string namespaceName;
    }

    template<typename TBase>
    TRegisterNamespaceTransaction<TBase> fromDto<TRegisterNamespaceTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TRegisterNamespaceTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));

        transaction.namespaceType = dto.value<"namespaceType"_>();
        transaction.durationOrParentId = dto.value<"durationOrParentId"_>();
        transaction.namespaceId = dto.value<"namespaceId"_>();
        transaction.namespaceNameSize = dto.value<"namespaceNameSize"_>();
        transaction.namespaceName = dto.value<"namespaceName"_>();

        return transaction;
    }

    template<typename TBase>
    class TSecretLockTransaction: public TBase {
    public:
        TSecretLockTransaction(const TBase& other) : TBase(other) {}
    public:
        MosaicId mosaicId;
        Amount amount;
        BlockDuration duration;
        SecretHashAlgorithm hashAlgorithm;
        Hash256 secret;
        AddressData recipient;
    };

    template<typename TBase>
    TSecretLockTransaction<TBase> fromDto<TSecretLockTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TSecretLockTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));

        transaction.mosaicId = dto.value<"mosaicId"_>();
        transaction.amount = dto.value<"amount"_>();
        transaction.duration = dto.value<"duration"_>();
        transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
        transaction.secret = dto.value<"secret"_>();
        transaction.recipient = dto.value<"recipient"_>();


        return transaction;
    }

    template<typename TBase>
    class TSecretProofTransaction: public TBase {
    public:
        TSecretProofTransaction(const TBase& other) : TBase(other) {}
    public:

        SecretHashAlgorithm hashAlgorithm;
        Hash256 secret;
        uint16_t proofSize;
        std::vector<uint8_t> proof;
    };

    template<typename TBase>
    TSecretProofTransaction<TBase> fromDto<TSecretProofTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TSecretProofTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));

        transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
        transaction.secret = dto.value<"secret"_>();
        transaction.proofSize = dto.value<"proofSize"_>();
        transaction.proof = dto.value<"proof"_>();


        return transaction;
    }


    template<typename TBase>
    class TTransferTransaction: public TBase {
    public:
        TTransferTransaction(const TBase& other) : TBase(other) {}
    public:
        AddressData recipient;
        uint16_t messageSize;
        uint8_t mosaicsCount;
        std::vector<uint8_t> message;
        std::vector<Mosaic> mosaics;

    };

    template<typename TBase>
    TTransferTransaction<TBase> fromDto<TTransferTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TTransferTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));

        transaction.recipient = dto.value<"recipient"_>();
        transaction.messageSize = dto.value<"messageSize"_>();
        transaction.mosaicsCount = dto.value<"mosaicsCount"_>();
        transaction.message = dto.value<"message"_>();
        transaction.mosaics = dto.value<"mosaics"_>();

        return transaction;
    }

    template<typename TBase>
    class TAliasTransactionBase: public TBase {
    public:
        TAliasTransactionBase(const TBase& other) : TBase(other) {}

    public:
        AliasTransactionAction aliasAction;
        NamespaceId namespaceId;
    };

    template<typename TBase>
    TAliasTransactionBase<TBase> fromDto<TAliasTransactionBase<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TAliasTransactionBase<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));

        transaction.aliasAction = dto.value<"aliasAction"_>();
        transaction.namespaceId = dto.value<"namespaceId"_>();

        return transaction;
    }

    template<typename TBase>
    class TAddressAliasTransaction: public TAliasTransactionBase<TBase> {
    public:
        TAddressAliasTransaction(const TBase& other) : TBase(other) {}
    public:
        AddressData address;
    };

    template<typename TBase>
    TAddressAliasTransaction<TBase> fromDto<TAddressAliasTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TAddressAliasTransaction<TBase> transaction(fromDto<TAliasTransactionBase<TBase>, GeneralTransactionDto>(dto));
        transaction.address = dto.value<"address"_>();
        return transaction;
    }

    template<typename TBase>
    class TMosaicAliasTransaction: public TAliasTransactionBase<TBase> {
    public:
        TMosaicAliasTransaction(const TAliasTransactionBase<TBase>& other) : TAliasTransactionBase<TBase>(other) {}
    public:
        MosaicId mosaicId
    };

    template<typename TBase>
    TMosaicAliasTransaction<TBase> fromDto<TMosaicAliasTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TAddressAliasTransaction<TBase> transaction(fromDto<TAliasTransactionBase<TBase>, GeneralTransactionDto>(dto));
        transaction.mosaicId = dto.value<"mosaicId"_>();
        return transaction;
    }

    template<typename TBase, typename T>
    class  TAccountPropertyTransaction: public TBase {
    public:
        TAccountPropertyTransaction(const TBase& other) : TBase(other) {}
    public:
        uint8_t propertyType;
        uint8_t modificationsCount;
        std::vector<TAccountPropertyModification<T>> modifications;
    };

    template<typename TBase, typename T>
    TAccountPropertyTransaction<TBase, T> fromDto<TAccountPropertyTransaction<TBase, T>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
        TAccountPropertyTransaction<TBase, T> transaction(fromDto<TBase>, GeneralTransactionDto>(dto));

        transaction.propertyType = dto.value<"propertyType"_>();
        transaction.modificationsCount = dto.value<"modificationsCount"_>();
        transaction.modifications = dto.value<"modifications"_>(); //??

        return transaction;
    }


    using AccountLinkTransaction  = TAccountLinkTransaction <Transaction >;
    using EmbeddedAccountLinkTransaction  = TAccountLinkTransaction <EmbeddedTransaction >;

    using LockFundsTransaction  = TLockFundsTransaction <Transaction >;
    using EmbeddedLockFundsTransaction  = TLockFundsTransaction <EmbeddedTransaction >;

    using ModifyMultisigAccountTransaction  = TModifyMultisigAccountTransaction <Transaction >;
    using EmbeddedModifyMultisigAccountTransaction  = TModifyMultisigAccountTransaction <EmbeddedTransaction >;

    using MosaicDefinitionTransaction  = TMosaicDefinitionTransaction <Transaction >;
    using EmbeddedMosaicDefinitionTransaction  = TMosaicDefinitionTransaction <EmbeddedTransaction >;

    using MosaicSupplyChangeTransaction  = TMosaicSupplyChangeTransaction <Transaction >;
    using EmbeddedMosaicSupplyChangeTransaction  = TMosaicSupplyChangeTransaction <EmbeddedTransaction >;

    using RegisterNamespaceTransaction  = TRegisterNamespaceTransaction <Transaction >;
    using EmbeddedRegisterNamespaceTransaction  = TRegisterNamespaceTransaction <EmbeddedTransaction >;

    using SecretLockTransaction  = TSecretLockTransaction <Transaction >;
    using EmbeddedSecretLockTransaction  = TSecretLockTransaction <EmbeddedTransaction >;

    using SecretProofTransaction  = TSecretProofTransaction <Transaction >;
    using EmbeddedSecretProofTransaction  = TSecretProofTransaction <EmbeddedTransaction >;

    using TransferTransaction  = TTransferTransaction <Transaction >;
    using EmbeddedTransferTransaction  = TTransferTransaction <EmbeddedTransaction >;

    using AddressAliasTransaction  = TAddressAliasTransaction <Transaction >;
    using EmbeddedAddressAliasTransaction  = TAddressAliasTransaction <EmbeddedTransaction >;

    using MosaicAliasTransaction  = TMosaicAliasTransaction <Transaction >;
    using EmbeddedMosaicAliasTransaction  = TMosaicAliasTransaction <EmbeddedTransaction >;

    using AccountAddressPropertyTransaction  = TAccountPropertyTransaction <Transaction , AddressData>;
    using EmbeddedAccountAddressPropertyTransaction  = TAccountPropertyTransaction <EmbeddedTransaction , AddressData>;

    using AccountMosaicPropertyTransaction  = TAccountPropertyTransaction <Transaction , MosaicId>;
    using EmbeddedAccountMosaicPropertyTransaction  = TAccountPropertyTransaction <EmbeddedTransaction , MosaicId>;

    using AccountTransactionPropertyTransaction  = TAccountPropertyTransaction<Transaction , TransactionType>;
    using EmbeddedAccountTransactionPropertyTransaction  = TAccountPropertyTransaction<EmbeddedTransaction , TransactionType>;
}





