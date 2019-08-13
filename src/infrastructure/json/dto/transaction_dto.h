
#pragma once

#include "infrastructure/json/descriptors.h"
#include "infrastructure/utils/variadic_struct.h"

#include <nemcpp/types.h>
#include <nemcpp/model/account/account_property.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/namespace/namespace.h>
#include <nemcpp/model/transaction_simple/account_link_transaction_types.h>
#include <nemcpp/model/transaction_simple/account_property_transaction_types.h>
#include <nemcpp/model/transaction_simple/alias_transaction_types.h>
#include <nemcpp/model/transaction_simple/modify_multisig_account_transaction_types.h>
#include <nemcpp/model/transaction_simple/mosaic_supply_change_transaction_types.h>
#include <nemcpp/model/transaction_simple/secret_hash_algorithm.h>
#include <nemcpp/model/transaction_simple/transaction_type.h>
#include "mosaic_dto.h"

#include <array>
#include <string>
#include <vector>

using namespace xpx_sdk::simple_transactions;

namespace xpx_sdk { namespace internal { namespace json {
	namespace dto {
	// Binary Dtos should contain only following types:
	//
	// 1. arithmetic (int, double, etc) and enums
	// 2. std::array which has fixed size
	// 3. std::string which requires size descriptor and treats it as string length
	// 4. std::vector which requires size descriptor and treats it as elements count
	// 5. other binary Dtos

	// Common Dtos
	//==========================================================================
		using MosaicPropertyDto = VariadicStruct<
				Field<STR_LITERAL("id"),    MosaicPropertyId>,
				Field<STR_LITERAL("value"), uint64_t>>;

		using CosignatureDto = VariadicStruct<
				Field<STR_LITERAL("publicKey"), Key>,
				Field<STR_LITERAL("signature"), Signature>>;

		using CosignatoryModificationDto = VariadicStruct<
				Field<STR_LITERAL("publicKey"),        Key>,
				Field<STR_LITERAL("modificationType"), CosignatoryModificationType>>;

		template<typename TValue>
		using TAccountPropertyModificationDto = VariadicStruct<
				Field<STR_LITERAL("value"),            TValue>,
				Field<STR_LITERAL("modificationType"), AccountPropertyModificationType>>;

		using AddressPropertyModificationDto = TAccountPropertyModificationDto<AddressData>;
		using MosaicPropertyModificationDto = TAccountPropertyModificationDto<MosaicId>;
		using TransactionPropertyModificationDto = TAccountPropertyModificationDto<TransactionType>;

		// Transaction Dtos
		//==========================================================================

		using BasicTransactionDto = VariadicStruct<
		        Field<STR_LITERAL("type"), TransactionType> >;

		using TransactionDto = VariadicStruct<
				Field<STR_LITERAL("size"),      uint32_t>,
				Field<STR_LITERAL("signature"), Signature>,
				Field<STR_LITERAL("signer"),    Key>,
				Field<STR_LITERAL("version"),   uint16_t>,
				Field<STR_LITERAL("type"),      TransactionType>,
				Field<STR_LITERAL("maxFee"),    Amount>,
				Field<STR_LITERAL("deadline"),  int64_t>>;

		using EmbeddedTransactionDto = VariadicStruct<
				Field<STR_LITERAL("size"),      uint32_t>,
				Field<STR_LITERAL("signer"),    Key>,
				Field<STR_LITERAL("version"),   uint16_t>,
				Field<STR_LITERAL("type"),      TransactionType>>;

		using AggregateTransactionDto = VariadicStruct<
				TransactionDto,
				Field<STR_LITERAL("payloadSize"),  uint32_t>,
				Field<STR_LITERAL("payload"),      std::vector<uint8_t> >,
				Field<STR_LITERAL("cosignatures"), std::vector<CosignatureDto> > >;

		template<typename TBase>
		using TAccountLinkTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("linkAction"),       AccountLinkTransactionAction>,
				Field<STR_LITERAL("remoteAccountKey"), Key>>;

		template<typename TBase>
		using TLockFundsTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("lockedMosaic"), MosaicDto>,
				Field<STR_LITERAL("lockDuration"), BlockDuration>,
				Field<STR_LITERAL("lockHash"),     Hash256>>;

		template<typename TBase>
		using TModifyMultisigAccountTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("minRemovalDelta"),    int8_t>,
				Field<STR_LITERAL("minApprovalDelta"),   int8_t>,
				Field<STR_LITERAL("modificationsCount"), uint8_t>,
				Field<STR_LITERAL("modifications"),      std::vector<CosignatoryModificationDto> > >;

		template<typename TBase>
		using TMosaicDefinitionTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("nonce"),                   uint32_t>,
				Field<STR_LITERAL("mosaicId"),                MosaicId>,
				Field<STR_LITERAL("optionalPropertiesCount"), uint8_t>,
				Field<STR_LITERAL("flags"),                   MosaicFlags>,
				Field<STR_LITERAL("divisibility"),            uint8_t>,
				Field<STR_LITERAL("optionalProperties"),      std::vector<MosaicPropertyDto> > >;

		template<typename TBase>
		using TMosaicSupplyChangeTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("mosaicId"),  MosaicId>,
				Field<STR_LITERAL("direction"), MosaicSupplyChangeDirection>,
				Field<STR_LITERAL("delta"),     Amount>>;

		template<typename TBase>
		using TRegisterNamespaceTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("namespaceType"),      NamespaceType>,
				Field<STR_LITERAL("durationOrParentId"), uint64_t>,
				Field<STR_LITERAL("namespaceId"),        NamespaceId>,
				Field<STR_LITERAL("namespaceNameSize"),  uint8_t>,
				Field<STR_LITERAL("namespaceName"),      std::string > >;

		template<typename TBase>
		using TSecretLockTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("mosaicId"),      MosaicId>,
				Field<STR_LITERAL("amount"),        Amount>,
				Field<STR_LITERAL("duration"),      BlockDuration>,
				Field<STR_LITERAL("hashAlgorithm"), SecretHashAlgorithm>,
				Field<STR_LITERAL("secret"),        Hash256>,
				Field<STR_LITERAL("recipient"),     AddressData>>;

		template<typename TBase>
		using TSecretProofTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("hashAlgorithm"), SecretHashAlgorithm>,
				Field<STR_LITERAL("secret"),        Hash256>,
				Field<STR_LITERAL("proofSize"),     uint16_t>,
				Field<STR_LITERAL("proof"),         std::vector<uint8_t> > >;

		template<typename TBase>
		using TTransferTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("recipient"),    AddressData>,
				Field<STR_LITERAL("messageSize"),  uint16_t>,
				Field<STR_LITERAL("mosaicsCount"), uint8_t>,
				Field<STR_LITERAL("message"),      std::vector<uint8_t> >,
				Field<STR_LITERAL("mosaics"),      std::vector<MosaicDto> > >;

		template<typename TBase>
		using TAliasTransactionBaseDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("aliasAction"), AliasTransactionAction>,
				Field<STR_LITERAL("namespaceId"), NamespaceId>>;

		template<typename TBase>
		using TAddressAliasTransactionDto = VariadicStruct<
				TAliasTransactionBaseDto<TBase>,
				Field<STR_LITERAL("address"), AddressData>>;

		template<typename TBase>
		using TMosaicAliasTransactionDto = VariadicStruct<
				TAliasTransactionBaseDto<TBase>,
				Field<STR_LITERAL("mosaicId"), MosaicId>>;

		template<typename TBase, typename T>
		using TAccountPropertyTransactionDto = VariadicStruct<
				TBase,
				Field<STR_LITERAL("propertyType"),       uint8_t>,
				Field<STR_LITERAL("modificationsCount"), uint8_t>,
				Field<STR_LITERAL("modifications"),      std::vector<TAccountPropertyModificationDto<T>> > >;

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

		using TransferTransactionDto = TTransferTransactionDto<TransactionDto>;
		using EmbeddedTransferTransactionDto = TTransferTransactionDto<EmbeddedTransactionDto>;

		using AddressAliasTransactionDto = TAddressAliasTransactionDto<TransactionDto>;
		using EmbeddedAddressAliasTransactionDto = TAddressAliasTransactionDto<EmbeddedTransactionDto>;

		using MosaicAliasTransactionDto = TMosaicAliasTransactionDto<TransactionDto>;
		using EmbeddedMosaicAliasTransactionDto = TMosaicAliasTransactionDto<EmbeddedTransactionDto>;

		using AccountAddressPropertyTransactionDto = TAccountPropertyTransactionDto<TransactionDto, AddressData>;
		using EmbeddedAccountAddressPropertyTransactionDto = TAccountPropertyTransactionDto<EmbeddedTransactionDto, AddressData>;

		using AccountMosaicPropertyTransactionDto = TAccountPropertyTransactionDto<TransactionDto, MosaicId>;
		using EmbeddedAccountMosaicPropertyTransactionDto = TAccountPropertyTransactionDto<EmbeddedTransactionDto, MosaicId>;

		using AccountTransactionPropertyTransactionDto = TAccountPropertyTransactionDto<TransactionDto, TransactionType>;
		using EmbeddedAccountTransactionPropertyTransactionDto = TAccountPropertyTransactionDto<EmbeddedTransactionDto, TransactionType>;

		using AliasTransactionBaseDto = TAliasTransactionBaseDto<TransactionDto>;
		using EmbeddedAliasTransactionBaseDto = TAliasTransactionBaseDto<EmbeddedTransactionDto>;


		using TransactionContainerDto = std::vector<std::string>;
	}}}}
