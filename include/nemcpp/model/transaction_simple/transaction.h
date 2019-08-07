#include <string>
#include <vector>
#include <stdint.h>
#include <nemcpp/types.h>
#include <nemcpp/model/transaction/transaction_type.h>
#include <nemcpp/model/transaction/alias_transaction_types.h>


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

    class EmbeddedTransaction {
    public:
        uint32_t size;
        Key signer;
        uint16_t version;
        TransactionType type;
    };

    class AggregateTransaction : public Transaction {
    public:
        AggregateTransaction(const Transaction& other) : Transaction(other) {}
    public:
        uint32_t payloadSize;
        std::vector<uint8_t> payload;
        std::vector<Signature> cosignatures; // Cosginatures is already container
    };

    template<typename TBase>
    class TAccountLinkTransaction : TBase {
    public:
        TAccountLinkTransaction(const TBase& other) : TBase(other) {}
    public:

        AccountLinkTransactionAction linkAction;
        Key remoteAccountKey;
    };


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
    class TModifyMultisigAccountTransaction : public TBase {
    public:
        TModifyMultisigAccountTransaction(const TBase& other) : TBase(other) {}
    public:
        int8_t minRemovalDelta;
        int8_t minApprovalDelta;
        uint8_t modificationsCount;
        std::vector<CosignatoryModifications> modifications; // CosignatoryModifications is already container
    };


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
    class TMosaicSupplyChangeTransaction: public TBase {
    public:
        TMosaicSupplyChangeTransaction(const TBase& other) : TBase(other) {}
    public:
        MosaicId mosaicId;
        MosaicSupplyChangeDirection direction;
        Amount delta;
    };



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
    };

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
    class TAliasTransactionBase: public TBase {
    public:
        TAliasTransactionBase(const TBase& other) : TBase(other) {}

    public:
        AliasTransactionAction aliasAction;
        NamespaceId namespaceId;
    };

    template<typename TBase>
    class TAddressAliasTransaction: public TAliasTransactionBase<TBase> {
    public:
        TAddressAliasTransaction(const TBase& other) : TBase(other) {}
    public:
        AddressData address;
    };

    template<typename TBase>
    class TMosaicAliasTransaction: public TAliasTransactionBase<TBase> {
    public:
        TMosaicAliasTransaction(const TAliasTransactionBase<TBase>& other) : TAliasTransactionBase<TBase>(other) {}
    public:
        MosaicId mosaicId;
    };

    template<typename TBase, typename T>
    class  TAccountPropertyTransaction: public TBase {
    public:
        TAccountPropertyTransaction(const TBase& other) : TBase(other) {}
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

    using AccountAddressPropertyTransaction  = TAccountPropertyTransaction<Transaction, AddressData>;
    using EmbeddedAccountAddressPropertyTransaction  = TAccountPropertyTransaction<EmbeddedTransaction, AddressData>;

    using AccountMosaicPropertyTransaction  = TAccountPropertyTransaction<Transaction, MosaicId>;
    using EmbeddedAccountMosaicPropertyTransaction  = TAccountPropertyTransaction <EmbeddedTransaction, MosaicId>;

    using AccountTransactionPropertyTransaction  = TAccountPropertyTransaction<Transaction, TransactionType>;
    using EmbeddedAccountTransactionPropertyTransaction  = TAccountPropertyTransaction<EmbeddedTransaction, TransactionType>;
}





