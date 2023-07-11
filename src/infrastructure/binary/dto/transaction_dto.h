/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include "infrastructure/binary/descriptors.h"
#include "infrastructure/utils/variadic_struct.h"

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/model/account/account_property.h>
#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <xpxchaincpp/model/namespace/namespace.h>
#include <xpxchaincpp/model/transaction/account_link_transaction_types.h>
#include <xpxchaincpp/model/transaction/account_property_transaction_types.h>
#include <xpxchaincpp/model/transaction/alias_transaction_types.h>
#include <xpxchaincpp/model/transaction/modify_multisig_account_transaction_types.h>
#include <xpxchaincpp/model/transaction/mosaic_supply_change_transaction_types.h>
#include <xpxchaincpp/model/transaction/secret_hash_algorithm.h>
#include <xpxchaincpp/model/transaction/transaction_type.h>

#include <array>
#include <string>
#include <vector>

using namespace xpx_chain_sdk;

namespace xpx_chain_sdk { namespace internal { namespace binary {
	
	// Binary DTOs should contain only following types:
	//
	// 1. arithmetic (int, double, etc) and enums
	// 2. std::array which has fixed size
	// 3. std::string which requires size descriptor and treats it as string length
	// 4. std::vector which requires size descriptor and treats it as elements count
	// 5. other binary DTOs
	
	// Common DTOs
	//==========================================================================
	
	using MosaicDTO = VariadicStruct<
		Field<STR_LITERAL("id"),     MosaicId>,
		Field<STR_LITERAL("amount"), Amount>>;
	
	using MosaicPropertyDTO = VariadicStruct<
		Field<STR_LITERAL("id"),    MosaicPropertyId>,
		Field<STR_LITERAL("value"), uint64_t>>;
	
	using CosignatureDTO = VariadicStruct<
		Field<STR_LITERAL("publicKey"), Key>,
		Field<STR_LITERAL("signature"), Signature>>;
	
	using CosignatoryModificationDTO = VariadicStruct<
		Field<STR_LITERAL("publicKey"),        Key>,
		Field<STR_LITERAL("modificationType"), CosignatoryModificationType>>;
	
	template<typename TValue>
	using TAccountPropertyModificationDTO = VariadicStruct<
		Field<STR_LITERAL("value"),            TValue>,
		Field<STR_LITERAL("modificationType"), AccountPropertyModificationType>>;
	
//	using AddressPropertyModificationDTO = TAccountPropertyModificationDTO<AccountAddressProperty::ValueType>;
//	using MosaicPropertyModificationDTO = TAccountPropertyModificationDTO<AccountMosaicProperty::ValueType>;
//	using TransactionPropertyModificationDTO = TAccountPropertyModificationDTO<AccountTransactionProperty::ValueType>;
	
	// Transaction DTOs
	//==========================================================================
	
	using TransactionDTO = VariadicStruct<
		Field<STR_LITERAL("size"),      uint32_t>,
		Field<STR_LITERAL("signature"), Signature>,
		Field<STR_LITERAL("signer"),    Key>,
		Field<STR_LITERAL("version"),   uint32_t>,
		Field<STR_LITERAL("type"),      TransactionType>,
		Field<STR_LITERAL("maxFee"),    Amount>,
		Field<STR_LITERAL("deadline"),  int64_t>>;
	
	using EmbeddedTransactionDTO = VariadicStruct<
		Field<STR_LITERAL("size"),      uint32_t>,
		Field<STR_LITERAL("signer"),    Key>,
		Field<STR_LITERAL("version"),   uint32_t>,
		Field<STR_LITERAL("type"),      TransactionType>>;
	
	using AggregateTransactionDTO = VariadicStruct<
		TransactionDTO,
		Field<STR_LITERAL("payloadSize"),  uint32_t>,
		Field<STR_LITERAL("payload"),      std::vector<uint8_t>,        desc::VariableSize<STR_LITERAL("payloadSize")>>,
		Field<STR_LITERAL("cosignatures"), std::vector<CosignatureDTO>, desc::TrailingSize<STR_LITERAL("size")>>>;
	
	template<typename TBase>
	using TAccountLinkTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("linkAction"),       AccountLinkTransactionAction>,
		Field<STR_LITERAL("remoteAccountKey"), Key>>;
	
	template<typename TBase>
	using TLockFundsTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("lockedMosaic"), MosaicDTO>,
		Field<STR_LITERAL("lockDuration"), BlockDuration>,
		Field<STR_LITERAL("lockHash"),     Hash256>>;
	
	template<typename TBase>
	using TModifyMultisigAccountTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("minRemovalDelta"),    int8_t>,
		Field<STR_LITERAL("minApprovalDelta"),   int8_t>,
		Field<STR_LITERAL("modificationsCount"), uint8_t>,
		Field<STR_LITERAL("modifications"),      std::vector<CosignatoryModificationDTO>, desc::VariableSize<STR_LITERAL("modificationsCount")>>>;
	
	template<typename TBase>
	using TMosaicDefinitionTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("mosaicNonce"),               uint32_t>,
		Field<STR_LITERAL("mosaicId"),                  MosaicId>,
        Field<STR_LITERAL("optionalPropertiesCount"),   uint8_t>,
        Field<STR_LITERAL("flags"),                     MosaicFlags>,
        Field<STR_LITERAL("divisibility"),              uint8_t>,
        Field<STR_LITERAL("optionalProperties"),        std::vector<MosaicPropertyDTO>, desc::VariableSize<STR_LITERAL("optionalPropertiesCount")>>>;

            template<typename TBase>
	using TMosaicSupplyChangeTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("mosaicId"),  MosaicId>,
		Field<STR_LITERAL("direction"), MosaicSupplyChangeDirection>,
		Field<STR_LITERAL("delta"),     Amount>>;
	
	template<typename TBase>
	using TRegisterNamespaceTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("namespaceType"),      NamespaceType>,
		Field<STR_LITERAL("durationOrParentId"), uint64_t>,
		Field<STR_LITERAL("namespaceId"),        NamespaceId>,
		Field<STR_LITERAL("namespaceNameSize"),  uint8_t>,
		Field<STR_LITERAL("namespaceName"),      std::string, desc::VariableSize<STR_LITERAL("namespaceNameSize")>>>;
	
	template<typename TBase>
	using TSecretLockTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("mosaicId"),      MosaicId>,
		Field<STR_LITERAL("amount"),        Amount>,
		Field<STR_LITERAL("duration"),      BlockDuration>,
		Field<STR_LITERAL("hashAlgorithm"), SecretHashAlgorithm>,
		Field<STR_LITERAL("secret"),        Hash256>,
		Field<STR_LITERAL("recipient"),     AddressData>>;
	
	template<typename TBase>
	using TSecretProofTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("hashAlgorithm"), SecretHashAlgorithm>,
		Field<STR_LITERAL("secret"),        Hash256>,
		Field<STR_LITERAL("proofSize"),     uint16_t>,
		Field<STR_LITERAL("proof"),         std::vector<uint8_t>, desc::VariableSize<STR_LITERAL("proofSize")>>>;

    template<typename TBase>
    using TStoragePaymentTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("driveKey"),      Key>,
        Field<STR_LITERAL("storageUnits"),  Amount>>;

	template<typename TBase>
	using TTransferTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("recipient"),    AddressData>,
		Field<STR_LITERAL("messageSize"),  uint16_t>,
		Field<STR_LITERAL("mosaicsCount"), uint8_t>,
		Field<STR_LITERAL("message"),      std::vector<uint8_t>,   desc::VariableSize<STR_LITERAL("messageSize")>>,
		Field<STR_LITERAL("mosaics"),      std::vector<MosaicDTO>, desc::VariableSize<STR_LITERAL("mosaicsCount")>>>;
	
	template<typename TBase>
	using TAliasTransactionBaseDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("aliasAction"), AliasTransactionAction>,
		Field<STR_LITERAL("namespaceId"), NamespaceId>>;
	
	template<typename TBase>
	using TAddressAliasTransactionDTO = VariadicStruct<
		TAliasTransactionBaseDTO<TBase>,
		Field<STR_LITERAL("address"), AddressData>>;
	
	template<typename TBase>
	using TMosaicAliasTransactionDTO = VariadicStruct<
		TAliasTransactionBaseDTO<TBase>,
		Field<STR_LITERAL("mosaicId"), MosaicId>>;
	
	template<typename TBase, typename T>
	using TAccountPropertyTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("propertyType"),       uint8_t>,
		Field<STR_LITERAL("modificationsCount"), uint8_t>,
		Field<STR_LITERAL("modifications"),      std::vector<TAccountPropertyModificationDTO<T>>, desc::VariableSize<STR_LITERAL("modificationsCount")>>>;

	template<typename TBase>
	using TPrepareBcDriveTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("driveSize"),			    uint64_t>,
		Field<STR_LITERAL("verificationFeeAmount"),	Amount>,
		Field<STR_LITERAL("replicatorCount"),		uint16_t>>;

    template<typename TBase>
    using TCreateLiquidityProviderTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("providerMosaicId"),		MosaicId>,
        Field<STR_LITERAL("currencyDeposit"),	    Amount>,
        Field<STR_LITERAL("initialMosaicsMinting"),	Amount>,
        Field<STR_LITERAL("slashingPeriod"),		uint32_t>,
        Field<STR_LITERAL("windowSize"),		    uint16_t>,
        Field<STR_LITERAL("slashingAccount"),		Key>,
        Field<STR_LITERAL("alpha"),		            uint32_t>,
        Field<STR_LITERAL("beta"),		            uint32_t>>;

    template<typename TBase>
    using TManualRateChangeTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("providerMosaicId"),          MosaicId>,
        Field<STR_LITERAL("currencyBalanceIncrease"),	bool>,
        Field<STR_LITERAL("currencyBalanceChange"),		Amount>,
        Field<STR_LITERAL("mosaicBalanceIncrease"),		bool>,
        Field<STR_LITERAL("mosaicBalanceChange"),		Amount>>;

	template<typename TBase>
	using TDataModificationTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("driveKey"),				Key>,
		Field<STR_LITERAL("downloadDataCdi"),		Hash256>,
		Field<STR_LITERAL("uploadSize"),			uint64_t>,
		Field<STR_LITERAL("feedbackFeeAmount"),		Amount>>;

	template<typename TBase>
	using TDownloadTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("driveKey"),				Key>,
		Field<STR_LITERAL("downloadSize"),			uint64_t>,
		Field<STR_LITERAL("feedbackFeeAmount"),		Amount>,
		Field<STR_LITERAL("listOfPublicKeysSize"),	uint16_t>,
		Field<STR_LITERAL("listOfPublicKeys"),	    std::vector<Key>, desc::VariableSize<STR_LITERAL("listOfPublicKeysSize")>>>;

    template<typename TBase>
    using TDownloadPaymentTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("downloadChannelId"),		Hash256>,
        Field<STR_LITERAL("downloadSize"),			uint64_t>,
        Field<STR_LITERAL("feedbackFeeAmount"),		Amount>>;

    template<typename TBase>
    using TDriveClosureTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("driveKey"), Key>>;

	template<typename TBase>
	using TDataModificationApprovalTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("driveKey"),				Key>,
		Field<STR_LITERAL("dataModificationId"),	Hash256>,
		Field<STR_LITERAL("fileStructureCdi"),		Hash256>,
		Field<STR_LITERAL("modificationStatus"),    uint8_t>,
		Field<STR_LITERAL("fileStructureSizeBytes"),uint64_t>,
		Field<STR_LITERAL("metaFilesSizeBytes"),	uint64_t>,
		Field<STR_LITERAL("usedDriveSizeBytes"),	uint64_t>,
		Field<STR_LITERAL("judgingKeysCount"),		uint8_t>,
		Field<STR_LITERAL("overlappingKeysCount"),	uint8_t>,
		Field<STR_LITERAL("judgedKeysCount"),		uint8_t>,
		Field<STR_LITERAL("opinionElementCount"),	uint16_t>,
		Field<STR_LITERAL("publicKeys"),		    std::vector<Key>, desc::VariableSize<STR_LITERAL("opinionElementCount")>>,
		Field<STR_LITERAL("signatures"),		    std::vector<Signature>, desc::VariableSize<STR_LITERAL("opinionElementCount")>>,
		Field<STR_LITERAL("presentOpinions"),		std::vector<uint8_t>, desc::VariableSize<STR_LITERAL("opinionElementCount")>>,
		Field<STR_LITERAL("opinions"),			    std::vector<uint64_t>, desc::VariableSize<STR_LITERAL("opinionElementCount")>>>;

	template<typename TBase>
	using TDataModificationCancelTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("driveKey"),				Key>,
		Field<STR_LITERAL("dataModificationId"),	Hash256>>;

    template<typename TBase>
    using TFinishDownloadTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("downloadChannelId"),	Hash256>,
        Field<STR_LITERAL("feedbackFeeAmount"),	Amount>>;

	template<typename TBase>
	using TReplicatorOnboardingTransactionDTO = VariadicStruct<
		TBase,
		Field<STR_LITERAL("capacity"), Amount>>;

    template<typename TBase>
        using TReplicatorOffboardingTransactionDTO = VariadicStruct<
        TBase,
        Field<STR_LITERAL("driveKey"), Key>>;

    template<typename TBase>
    using TDeployContractTransactionDTO = VariadicStruct<
            TBase,
            Field<STR_LITERAL("driveKey"), Key>,
            Field<STR_LITERAL("fileNameSize"),  uint16_t>,
            Field<STR_LITERAL("functionNameSize"),  uint16_t>,
            Field<STR_LITERAL("actualArgumentsSize"),  uint16_t>,
            Field<STR_LITERAL("executionCallPayment"), Amount>,
            Field<STR_LITERAL("downloadCallPayment"), Amount>,
            Field<STR_LITERAL("servicePaymentsCount"),  uint8_t>,
            Field<STR_LITERAL("automaticExecutionsFileNameSize"),  uint16_t>,
            Field<STR_LITERAL("automaticExecutionsFunctionNameSize"),  uint16_t>,
            Field<STR_LITERAL("automaticExecutionsCallPayment"), Amount>,
            Field<STR_LITERAL("automaticDownloadCallPayment"), Amount>,
            Field<STR_LITERAL("automaticExecutionsNumber"), uint32_t>,
            Field<STR_LITERAL("assignee"), Key>,
            Field<STR_LITERAL("fileName"), std::string, desc::VariableSize<STR_LITERAL("fileNameSize")>>,
            Field<STR_LITERAL("functionName"), std::string, desc::VariableSize<STR_LITERAL("functionNameSize")>>,
            Field<STR_LITERAL("actualArguments"), std::vector<uint8_t>, desc::VariableSize<STR_LITERAL("actualArgumentsSize")>>,
            Field<STR_LITERAL("servicePayments"), std::vector<MosaicDTO>, desc::VariableSize<STR_LITERAL("servicePaymentsCount")>>,
            Field<STR_LITERAL("automaticExecutionsFileName"), std::string, desc::VariableSize<STR_LITERAL("automaticExecutionsFileNameSize")>>,
            Field<STR_LITERAL("automaticExecutionsFunctionName"), std::string, desc::VariableSize<STR_LITERAL("automaticExecutionsFunctionNameSize")>>>;

    template<typename TBase>
    using TManualCallTransactionDTO = VariadicStruct<
            TBase,
            Field<STR_LITERAL("contractKey"), Key>,
            Field<STR_LITERAL("fileNameSize"),  uint16_t>,
            Field<STR_LITERAL("functionNameSize"),  uint16_t>,
            Field<STR_LITERAL("actualArgumentsSize"),  uint16_t>,
            Field<STR_LITERAL("executionCallPayment"), Amount>,
            Field<STR_LITERAL("downloadCallPayment"), Amount>,
            Field<STR_LITERAL("servicePaymentsCount"),  uint8_t>,
            Field<STR_LITERAL("fileName"), std::string, desc::VariableSize<STR_LITERAL("fileNameSize")>>,
            Field<STR_LITERAL("functionName"), std::string, desc::VariableSize<STR_LITERAL("functionNameSize")>>,
            Field<STR_LITERAL("actualArguments"), std::vector<uint8_t>, desc::VariableSize<STR_LITERAL("actualArgumentsSize")>>,
            Field<STR_LITERAL("servicePayments"), std::vector<MosaicDTO>, desc::VariableSize<STR_LITERAL("servicePaymentsCount")>>>;

    template<typename TBase>
    using TAutomaticExecutionsPaymentTransactionDTO = VariadicStruct<
            TBase,
            Field<STR_LITERAL("contractKey"), Key>,
            Field<STR_LITERAL("automaticExecutionsNumber"), uint32_t>>;

    template<typename TBase>
    using TStreamStartTransactionDTO = VariadicStruct<
            TBase,
            Field<STR_LITERAL("driveKey"), Key>,
            Field<STR_LITERAL("expectedUploadSizeMegabytes"), uint64_t>,
            Field<STR_LITERAL("folderNameSize"), uint16_t>,
            Field<STR_LITERAL("feedbackFeeAmount"), uint64_t>,
            Field<STR_LITERAL("folderName"), std::vector<uint8_t>, desc::VariableSize<STR_LITERAL("folderNameSize")>>>;

    template<typename TBase>
    using TStreamFinishTransactionDTO = VariadicStruct<
            TBase,
            Field<STR_LITERAL("driveKey"), Key>,
            Field<STR_LITERAL("streamId"), Key>,
            Field<STR_LITERAL("actualUploadSize"), uint64_t>,
            Field<STR_LITERAL("streamStructureCdi"), Key>>;

    template<typename TBase>
    using TStreamPaymentTransactionDTO = VariadicStruct<
            TBase,
            Field<STR_LITERAL("driveKey"), Key>,
            Field<STR_LITERAL("streamId"), Key>,
            Field<STR_LITERAL("additionalUploadSize"), uint64_t>>;

	using AccountLinkTransactionDTO = TAccountLinkTransactionDTO<TransactionDTO>;
	using EmbeddedAccountLinkTransactionDTO = TAccountLinkTransactionDTO<EmbeddedTransactionDTO>;
	
	using LockFundsTransactionDTO = TLockFundsTransactionDTO<TransactionDTO>;
	using EmbeddedLockFundsTransactionDTO = TLockFundsTransactionDTO<EmbeddedTransactionDTO>;
	
	using ModifyMultisigAccountTransactionDTO = TModifyMultisigAccountTransactionDTO<TransactionDTO>;
	using EmbeddedModifyMultisigAccountTransactionDTO = TModifyMultisigAccountTransactionDTO<EmbeddedTransactionDTO>;
	
	using MosaicDefinitionTransactionDTO = TMosaicDefinitionTransactionDTO<TransactionDTO>;
	using EmbeddedMosaicDefinitionTransactionDTO = TMosaicDefinitionTransactionDTO<EmbeddedTransactionDTO>;
	
	using MosaicSupplyChangeTransactionDTO = TMosaicSupplyChangeTransactionDTO<TransactionDTO>;
	using EmbeddedMosaicSupplyChangeTransactionDTO = TMosaicSupplyChangeTransactionDTO<EmbeddedTransactionDTO>;
	
	using RegisterNamespaceTransactionDTO = TRegisterNamespaceTransactionDTO<TransactionDTO>;
	using EmbeddedRegisterNamespaceTransactionDTO = TRegisterNamespaceTransactionDTO<EmbeddedTransactionDTO>;
	
	using SecretLockTransactionDTO = TSecretLockTransactionDTO<TransactionDTO>;
	using EmbeddedSecretLockTransactionDTO = TSecretLockTransactionDTO<EmbeddedTransactionDTO>;
	
	using SecretProofTransactionDTO = TSecretProofTransactionDTO<TransactionDTO>;
	using EmbeddedSecretProofTransactionDTO = TSecretProofTransactionDTO<EmbeddedTransactionDTO>;

    using StoragePaymentTransactionDTO = TStoragePaymentTransactionDTO<TransactionDTO>;
    using EmbeddedStoragePaymentTransactionDTO = TStoragePaymentTransactionDTO<EmbeddedTransactionDTO>;

	using TransferTransactionDTO = TTransferTransactionDTO<TransactionDTO>;
	using EmbeddedTransferTransactionDTO = TTransferTransactionDTO<EmbeddedTransactionDTO>;
	
	using AddressAliasTransactionDTO = TAddressAliasTransactionDTO<TransactionDTO>;
	using EmbeddedAddressAliasTransactionDTO = TAddressAliasTransactionDTO<EmbeddedTransactionDTO>;
	
	using MosaicAliasTransactionDTO = TMosaicAliasTransactionDTO<TransactionDTO>;
	using EmbeddedMosaicAliasTransactionDTO = TMosaicAliasTransactionDTO<EmbeddedTransactionDTO>;
	
	using AccountAddressPropertyTransactionDTO = TAccountPropertyTransactionDTO<TransactionDTO, AddressData>;
	using EmbeddedAccountAddressPropertyTransactionDTO = TAccountPropertyTransactionDTO<EmbeddedTransactionDTO, AddressData>;
	
	using AccountMosaicPropertyTransactionDTO = TAccountPropertyTransactionDTO<TransactionDTO, MosaicId>;
	using EmbeddedAccountMosaicPropertyTransactionDTO = TAccountPropertyTransactionDTO<EmbeddedTransactionDTO, MosaicId>;
	
	using AccountTransactionPropertyTransactionDTO = TAccountPropertyTransactionDTO<TransactionDTO, TransactionType>;
	using EmbeddedAccountTransactionPropertyTransactionDTO = TAccountPropertyTransactionDTO<EmbeddedTransactionDTO, TransactionType>;

	using PrepareBcDriveTransactionDTO = TPrepareBcDriveTransactionDTO<TransactionDTO>;
	using EmbeddedPrepareBcDriveTransactionDTO = TPrepareBcDriveTransactionDTO<EmbeddedTransactionDTO>;

    using CreateLiquidityProviderTransactionDTO = TCreateLiquidityProviderTransactionDTO<TransactionDTO>;
    using EmbeddedCreateLiquidityProviderTransactionDTO = TCreateLiquidityProviderTransactionDTO<EmbeddedTransactionDTO>;

    using ManualRateChangeTransactionDTO = TManualRateChangeTransactionDTO<TransactionDTO>;
    using EmbeddedManualRateChangeTransactionDTO = TManualRateChangeTransactionDTO<EmbeddedTransactionDTO>;

	using DataModificationTransactionDTO = TDataModificationTransactionDTO<TransactionDTO>;
	using EmbeddedDataModificationTransactionDTO = TDataModificationTransactionDTO<EmbeddedTransactionDTO>;

	using DownloadTransactionDTO = TDownloadTransactionDTO<TransactionDTO>;
	using EmbeddedDownloadTransactionDTO = TDownloadTransactionDTO<EmbeddedTransactionDTO>;

    using DownloadPaymentTransactionDTO = TDownloadPaymentTransactionDTO<TransactionDTO>;
    using EmbeddedDownloadPaymentTransactionDTO = TDownloadPaymentTransactionDTO<EmbeddedTransactionDTO>;

    using DriveClosureTransactionDTO = TDriveClosureTransactionDTO<TransactionDTO>;
    using EmbeddedDriveClosureTransactionDTO = TDriveClosureTransactionDTO<EmbeddedTransactionDTO>;

	using DataModificationApprovalTransactionDTO = TDataModificationApprovalTransactionDTO<TransactionDTO>;
	using EmbeddedDataModificationApprovalTransactionDTO = TDataModificationApprovalTransactionDTO<EmbeddedTransactionDTO>;

	using DataModificationCancelTransactionDTO = TDataModificationCancelTransactionDTO<TransactionDTO>;
	using EmbeddedDataModificationCancelTransactionDTO = TDataModificationCancelTransactionDTO<EmbeddedTransactionDTO>;

    using FinishDownloadTransactionDTO = TFinishDownloadTransactionDTO<TransactionDTO>;
    using EmbeddedFinishDownloadTransactionDTO = TFinishDownloadTransactionDTO<EmbeddedTransactionDTO>;

	using ReplicatorOnboardingTransactionDTO = TReplicatorOnboardingTransactionDTO<TransactionDTO>;
	using EmbeddedReplicatorOnboardingTransactionDTO = TReplicatorOnboardingTransactionDTO<EmbeddedTransactionDTO>;

    using ReplicatorOffboardingTransactionDTO = TReplicatorOffboardingTransactionDTO<TransactionDTO>;
    using EmbeddedReplicatorOffboardingTransactionDTO = TReplicatorOffboardingTransactionDTO<EmbeddedTransactionDTO>;

    using DeployContractTransactionDTO = TDeployContractTransactionDTO<TransactionDTO>;
    using EmbeddedDeployContractTransactionDTO = TDeployContractTransactionDTO<EmbeddedTransactionDTO>;

    using ManualCallTransactionDTO = TManualCallTransactionDTO<TransactionDTO>;
    using EmbeddedManualCallTransactionDTO = TManualCallTransactionDTO<EmbeddedTransactionDTO>;

    using AutomaticExecutionsPaymentTransactionDTO = TAutomaticExecutionsPaymentTransactionDTO<TransactionDTO>;
    using EmbeddedAutomaticExecutionsPaymentTransactionDTO = TAutomaticExecutionsPaymentTransactionDTO<EmbeddedTransactionDTO>;

    using StreamStartTransactionDTO = TStreamStartTransactionDTO<TransactionDTO>;
    using EmbeddedStreamStartTransactionDTO = TStreamStartTransactionDTO<EmbeddedTransactionDTO>;

    using StreamFinishTransactionDTO = TStreamFinishTransactionDTO<TransactionDTO>;
    using EmbeddedStreamFinishTransactionDTO = TStreamFinishTransactionDTO<EmbeddedTransactionDTO>;

    using StreamPaymentTransactionDTO = TStreamPaymentTransactionDTO<TransactionDTO>;
    using EmbeddedStreamPaymentTransactionDTO = TStreamPaymentTransactionDTO<EmbeddedTransactionDTO>;
}}}
