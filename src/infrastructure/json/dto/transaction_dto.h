/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include "infrastructure/json/descriptors.h"
#include "infrastructure/json/uint32.h"
#include "infrastructure/utils/variadic_struct.h"

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/model/account/account_property.h>
#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/namespace/namespace.h>
#include <xpxchaincpp/model/transaction/account_link_transaction_types.h>
#include <xpxchaincpp/model/transaction/account_property_transaction_types.h>
#include <xpxchaincpp/model/transaction/alias_transaction_types.h>
#include <xpxchaincpp/model/transaction/modify_multisig_account_transaction_types.h>
#include <xpxchaincpp/model/transaction/mosaic_supply_change_transaction_types.h>
#include <xpxchaincpp/model/transaction/secret_hash_algorithm.h>
#include <xpxchaincpp/model/transaction/transaction_type.h>
#include "mosaic_dto.h"
#include <infrastructure/json/hex.h>

#include <array>
#include <string>
#include <vector>


namespace xpx_chain_sdk::internal::json::dto {
	// Json Dtos should contain only following types:
	//
	// 1. arithmetic (int, double, etc) and enums
	// 2. std::array which has fixed size
	// 3. std::string which requires size descriptor and treats it as string length
	// 4. std::vector which requires size descriptor and treats it as elements count
	// 5. other json Dtos

	// Common Dtos
	//==========================================================================

        using Uint32 = xpx_chain_sdk::internal::json::Uint32;
        using Uint64 = xpx_chain_sdk::internal::json::Uint64;

		using MosaicPropertyDto = VariadicStruct<
				Field<STR_LITERAL("id"),    MosaicPropertyId>,
				Field<STR_LITERAL("value"), Uint64>>;

		using CosignatureDto = VariadicStruct<
				Field<STR_LITERAL("publicKey"), std::string>,
				Field<STR_LITERAL("signature"), std::string>>;

		using CosignatoryModificationDto = VariadicStruct<
				Field<STR_LITERAL("publicKey"),        std::string>,
				Field<STR_LITERAL("modificationType"), CosignatoryModificationType>>;

		template<typename TValue>
		using TAccountPropertyModificationDto = VariadicStruct<
				Field<STR_LITERAL("value"),            TValue>,
				Field<STR_LITERAL("modificationType"), AccountPropertyModificationType>>;



		//Transaction meta Dtos
        //==========================================================================

		using TransactionInfoDto = VariadicStruct<
                Field<STR_LITERAL("height"), Uint64>,
                Field<STR_LITERAL("index"), Uint32>,
                Field<STR_LITERAL("id"), std::string>,
                Field<STR_LITERAL("hash"), std::string>,
                Field<STR_LITERAL("merkleComponentHash"), std::string>,
                Field<STR_LITERAL("aggregateHash"), std::string>,
                Field<STR_LITERAL("uniqueAggregateHash"), std::string>,
                Field<STR_LITERAL("aggregateId"), std::string>>;

		using TransactionStatusDto = VariadicStruct<
		        Field<STR_LITERAL("group"), std::string>,
				Field<STR_LITERAL("status"), std::string>,
				Field<STR_LITERAL("hash"), std::string>,
				Field<STR_LITERAL("deadline"), Uint64 > >;

		using MultipleTransactionInfoDto = std::vector<TransactionInfoDto>;
		using MultipleTransactionStatusDto = std::vector<TransactionStatusDto>;

		// Transaction Dtos
		//==========================================================================

		using BasicTransactionDto = VariadicStruct<
		        Field<STR_LITERAL("type"), TransactionType> >;

		using TransactionDto = VariadicStruct<
				Field<STR_LITERAL("signature"), std::string>,
				Field<STR_LITERAL("signer"),    std::string>,
				Field<STR_LITERAL("version"),   Uint32>,
				Field<STR_LITERAL("type"),      TransactionType>,
				Field<STR_LITERAL("maxFee"),    Uint64 >,
				Field<STR_LITERAL("deadline"),  Uint64> >;

		using EmbeddedTransactionDto = VariadicStruct<
				Field<STR_LITERAL("signer"),    std::string>,
				Field<STR_LITERAL("version"),   Uint32>,
				Field<STR_LITERAL("type"),      TransactionType>>;

		using AggregateTransactionDto = VariadicStruct<
				TransactionDto,
				Field<STR_LITERAL("payloadSize"),  Uint32>,
				Field<STR_LITERAL("payload"),      std::vector<uint8_t> >,
				Field<STR_LITERAL("cosignatures"), std::vector<CosignatureDto> > >;

		template<typename TBase>
		using TAccountLinkTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("linkAction"),       AccountLinkTransactionAction>,
				Field<STR_LITERAL("remoteAccountKey"), std::string>>;

		template<typename TBase>
		using TLockFundsTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("lockedMosaic"), MosaicDto>,
				Field<STR_LITERAL("lockDuration"), BlockDuration>,
				Field<STR_LITERAL("lockHash"),     std::string >>;

		template<typename TBase>
		using TModifyMultisigAccountTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("minRemovalDelta"),    int8_t>,
				Field<STR_LITERAL("minApprovalDelta"),   int8_t>,
				Field<STR_LITERAL("modifications"),      std::vector<CosignatoryModificationDto> > >;

		template<typename TBase>
		using TMosaicDefinitionTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("nonce"),                   Uint32>,
				Field<STR_LITERAL("mosaicId"),                Uint64>,
				Field<STR_LITERAL("flags"),                   MosaicFlags>,
				Field<STR_LITERAL("divisibility"),            uint8_t>,
				Field<STR_LITERAL("optionalProperties"),      std::vector<MosaicPropertyDto> > >;

		template<typename TBase>
		using TMosaicSupplyChangeTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("mosaicId"),  Uint64>,
				Field<STR_LITERAL("direction"), MosaicSupplyChangeDirection>,
				Field<STR_LITERAL("delta"),     Uint64>>;

		template<typename TBase>
		using TRegisterNamespaceTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("namespaceType"),      NamespaceType>,
				Field<STR_LITERAL("durationOrParentId"), Uint64>,
				Field<STR_LITERAL("namespaceId"),        NamespaceId>,
				Field<STR_LITERAL("namespaceNameSize"),  uint8_t>,
				Field<STR_LITERAL("namespaceName"),      std::string > >;

		template<typename TBase>
		using TSecretLockTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("mosaicId"),      Uint64>,
				Field<STR_LITERAL("amount"),        Uint64>,
				Field<STR_LITERAL("duration"),      BlockDuration>,
				Field<STR_LITERAL("hashAlgorithm"), SecretHashAlgorithm>,
				Field<STR_LITERAL("secret"),        std::string >,
				Field<STR_LITERAL("recipient"),     std::string>>;

		template<typename TBase>
		using TSecretProofTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("hashAlgorithm"), SecretHashAlgorithm>,
				Field<STR_LITERAL("secret"),        std::string >,
				Field<STR_LITERAL("proof"),         std::vector<uint8_t> > >;

        template<typename TBase>
        using TStoragePaymentTransactionDto = VariadicStruct<
                TBase,
                Field<STR_LITERAL("driveKey"),      std::string>,
                Field<STR_LITERAL("storageUnits"),  Uint64>>;

		using TransferTransactionMessageDto = VariadicStruct<
                Field<STR_LITERAL("type"),        Uint32 >,
                Field<STR_LITERAL("payload"),     std::string > >;

		template<typename TBase>
		using TTransferTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("recipient"),    std::string>,
				Field<STR_LITERAL("message"),      TransferTransactionMessageDto, desc::Optional>,
				Field<STR_LITERAL("mosaics"),      std::vector<MosaicDto> > >;

		template<typename TBase>
		using TAliasTransactionBaseDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("aliasAction"), AliasTransactionAction>,
				Field<STR_LITERAL("namespaceId"), NamespaceId>>;

		template<typename TBase>
		using TAddressAliasTransactionDto = VariadicStruct<
				TAliasTransactionBaseDto<TBase>,
				Field<STR_LITERAL("address"), std::string>>;

		template<typename TBase>
		using TMosaicAliasTransactionDto = VariadicStruct<
				TAliasTransactionBaseDto<TBase>,
				Field<STR_LITERAL("mosaicId"), Uint64>>;

		template<typename TBase, typename T>
		using TAccountPropertyTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("propertyType"),       uint8_t>,
				Field<STR_LITERAL("modificationsCount"), uint8_t>,
				Field<STR_LITERAL("modifications"),      std::vector<TAccountPropertyModificationDto<T>> > >;

		template<typename TBase>
		using TPrepareBcDriveTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("driveSize"),				Uint64>,
				Field<STR_LITERAL("verificationFeeAmount"), Uint64>,
				Field<STR_LITERAL("replicatorCount"),		uint16_t> >;

		template<typename TBase>
		using TDataModificationTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("driveKey"),				std::string>,
				Field<STR_LITERAL("downloadDataCdi"),		std::string>,
				Field<STR_LITERAL("uploadSize"),			Uint64>,
				Field<STR_LITERAL("feedbackFeeAmount"),		Uint64> >;

		template<typename TBase>
		using TDownloadTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("driveKey"),				std::string>,
				Field<STR_LITERAL("downloadSize"),			Uint64>,
				Field<STR_LITERAL("feedbackFeeAmount"),		Uint64>,
				Field<STR_LITERAL("listOfPublicKeysSize"),	uint16_t>,
                Field<STR_LITERAL("listOfPublicKeys"),	    std::vector<std::string> >>;

        template<typename TBase>
        using TDownloadPaymentTransactionDto = VariadicStruct<
                TBase,
                Field<STR_LITERAL("downloadChannelId"),		std::string>,
                Field<STR_LITERAL("downloadSize"),			Uint64>,
                Field<STR_LITERAL("feedbackFeeAmount"),		Uint64>>;

        template<typename TBase>
        using TDriveClosureTransactionDto = VariadicStruct<
                TBase,
                Field<STR_LITERAL("driveKey"), std::string>>;

		template<typename TBase>
		using TDataModificationApprovalTransactionDto = VariadicStruct<
				TBase,
                Field<STR_LITERAL("driveKey"),				std::string>,
                Field<STR_LITERAL("dataModificationId"),	std::string>,
                Field<STR_LITERAL("fileStructureCdi"),		std::string>,
                Field<STR_LITERAL("fileStructureSizeBytes"),		Uint64>,
                Field<STR_LITERAL("metaFilesSizeBytes"),		    Uint64>,
                Field<STR_LITERAL("usedDriveSizeBytes"),		    Uint64>,
                Field<STR_LITERAL("publicKeys"),		    std::vector<std::string>>,
                Field<STR_LITERAL("signatures"),		    std::vector<std::string>>,
                Field<STR_LITERAL("presentOpinions"),		std::vector<int64_t>>,
                Field<STR_LITERAL("opinions"),			    std::vector<Uint64>>>;

		template<typename TBase>
		using TDataModificationCancelTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("driveKey"),				std::string>,
				Field<STR_LITERAL("dataModificationId"),	std::string>>;

        template<typename TBase>
        using TFinishDownloadTransactionDto = VariadicStruct<
                TBase,
                Field<STR_LITERAL("downloadChannelId"),	std::string>,
                Field<STR_LITERAL("feedbackFeeAmount"),	Uint64>>;

		template<typename TBase>
		using TReplicatorOnboardingTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("capacity"), Uint64>>;

        template<typename TBase>
        using TReplicatorOffboardingTransactionDto = VariadicStruct<
                TBase,
                Field<STR_LITERAL("driveKey"), std::string>>;

		using AccountLinkTransactionDto = TAccountLinkTransactionDto<TransactionDto>;
		using EmbeddedAccountLinkTransactionDto = TAccountLinkTransactionDto<EmbeddedTransactionDto>;

		using LockFundsTransactionDto = TLockFundsTransactionDto<TransactionDto>;
		using EmbeddedLockFundsTransactionDto = TLockFundsTransactionDto<EmbeddedTransactionDto>;

		using ModifyMultisigAccountTransactionDto = TModifyMultisigAccountTransactionDto<TransactionDto>;
		using EmbeddedModifyMultisigAccountTransactionDto = TModifyMultisigAccountTransactionDto<EmbeddedTransactionDto>;

		using MosaicDefinitionTransactionDto = TMosaicDefinitionTransactionDto<TransactionDto>;
		using EmbeddedMosaicDefinitionTransactionDto = TMosaicDefinitionTransactionDto<EmbeddedTransactionDto>;

		using MosaicSupplyChangeTransactionDto = TMosaicSupplyChangeTransactionDto<TransactionDto>;
		using EmbeddedMosaicSupplyChangeTransactionDto = TMosaicSupplyChangeTransactionDto<EmbeddedTransactionDto>;

		using RegisterNamespaceTransactionDto = TRegisterNamespaceTransactionDto<TransactionDto>;
		using EmbeddedRegisterNamespaceTransactionDto = TRegisterNamespaceTransactionDto<EmbeddedTransactionDto>;

		using SecretLockTransactionDto = TSecretLockTransactionDto<TransactionDto>;
		using EmbeddedSecretLockTransactionDto = TSecretLockTransactionDto<EmbeddedTransactionDto>;

		using SecretProofTransactionDto = TSecretProofTransactionDto<TransactionDto>;
		using EmbeddedSecretProofTransactionDto = TSecretProofTransactionDto<EmbeddedTransactionDto>;

        using StoragePaymentTransactionDto = TStoragePaymentTransactionDto<TransactionDto>;
        using EmbeddedStoragePaymentTransactionDto = TStoragePaymentTransactionDto<EmbeddedTransactionDto>;

		using TransferTransactionDto = TTransferTransactionDto<TransactionDto>;
		using EmbeddedTransferTransactionDto = TTransferTransactionDto<EmbeddedTransactionDto>;

		using AddressAliasTransactionDto = TAddressAliasTransactionDto<TransactionDto>;
		using EmbeddedAddressAliasTransactionDto = TAddressAliasTransactionDto<EmbeddedTransactionDto>;

		using MosaicAliasTransactionDto = TMosaicAliasTransactionDto<TransactionDto>;
		using EmbeddedMosaicAliasTransactionDto = TMosaicAliasTransactionDto<EmbeddedTransactionDto>;

		using AccountAddressPropertyTransactionDto = TAccountPropertyTransactionDto<TransactionDto, std::string>;
		using EmbeddedAccountAddressPropertyTransactionDto = TAccountPropertyTransactionDto<EmbeddedTransactionDto, std::string>;

		using AccountMosaicPropertyTransactionDto = TAccountPropertyTransactionDto<TransactionDto, MosaicId>;
		using EmbeddedAccountMosaicPropertyTransactionDto = TAccountPropertyTransactionDto<EmbeddedTransactionDto, MosaicId>;

		using AccountTransactionPropertyTransactionDto = TAccountPropertyTransactionDto<TransactionDto, TransactionType>;
		using EmbeddedAccountTransactionPropertyTransactionDto = TAccountPropertyTransactionDto<EmbeddedTransactionDto, TransactionType>;

		using AliasTransactionBaseDto = TAliasTransactionBaseDto<TransactionDto>;
		using EmbeddedAliasTransactionBaseDto = TAliasTransactionBaseDto<EmbeddedTransactionDto>;

		using TransactionContainerDto = std::vector<std::string>;

		using AddressPropertyModificationDto = TAccountPropertyModificationDto<std::string>; // AddressData
		using MosaicPropertyModificationDto = TAccountPropertyModificationDto<MosaicId>;
		using TransactionPropertyModificationDto = TAccountPropertyModificationDto<TransactionType>;

		using PrepareBcDriveTransactionDto = TPrepareBcDriveTransactionDto<TransactionDto>;
		using EmbeddedPrepareBcDriveTransactionDto = TPrepareBcDriveTransactionDto<EmbeddedTransactionDto>;

		using DataModificationTransactionDto = TDataModificationTransactionDto<TransactionDto>;
		using EmbeddedDataModificationTransactionDto = TDataModificationTransactionDto<EmbeddedTransactionDto>;

		using DownloadTransactionDto = TDownloadTransactionDto<TransactionDto>;
		using EmbeddedDownloadTransactionDto = TDownloadTransactionDto<EmbeddedTransactionDto>;

        using DownloadPaymentTransactionDto = TDownloadPaymentTransactionDto<TransactionDto>;
        using EmbeddedDownloadPaymentTransactionDto = TDownloadPaymentTransactionDto<EmbeddedTransactionDto>;

        using DriveClosureTransactionDto = TDriveClosureTransactionDto<TransactionDto>;
        using EmbeddedDriveClosureTransactionDto = TDriveClosureTransactionDto<EmbeddedTransactionDto>;

		using DataModificationApprovalTransactionDto = TDataModificationApprovalTransactionDto<TransactionDto>;
		using EmbeddedDataModificationApprovalTransactionDto = TDataModificationApprovalTransactionDto<EmbeddedTransactionDto>;

		using DataModificationCancelTransactionDto = TDataModificationCancelTransactionDto<TransactionDto>;
		using EmbeddedDataModificationCancelTransactionDto = TDataModificationCancelTransactionDto<EmbeddedTransactionDto>;

        using FinishDownloadTransactionDto = TFinishDownloadTransactionDto<TransactionDto>;
        using EmbeddedFinishDownloadTransactionDto = TFinishDownloadTransactionDto<EmbeddedTransactionDto>;

		using ReplicatorOnboardingTransactionDto = TReplicatorOnboardingTransactionDto<TransactionDto>;
		using EmbeddedReplicatorOnboardingTransactionDto = TReplicatorOnboardingTransactionDto<EmbeddedTransactionDto>;

        using ReplicatorOffboardingTransactionDto = TReplicatorOffboardingTransactionDto<TransactionDto>;
        using EmbeddedReplicatorOffboardingTransactionDto = TReplicatorOffboardingTransactionDto<EmbeddedTransactionDto>;
	}
