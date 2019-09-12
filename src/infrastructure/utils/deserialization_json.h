#pragma once

#include <infrastructure/json/parser.h>
#include <infrastructure/json/dto/block_dto.h>
#include <infrastructure/json/dto/mosaic_dto.h>
#include <infrastructure/json/dto/namespace_dto.h>
#include <infrastructure/json/dto/height.h>
#include <infrastructure/json/dto/account_info_dto.h>
#include <infrastructure/json/dto/multiple_namespace_dto.h>
#include <infrastructure/json/dto/multiple_blocks_dto.h>
#include <infrastructure/json/dto/account_info_dto.h>
#include <infrastructure/json/dto/multisig_graph_dto.h>
#include <infrastructure/json/dto/network_info_dto.h>
#include <infrastructure/json/dto/transaction_dto.h>

#include <xpxchaincpp/model/blockchain/block.h>
#include <xpxchaincpp/model/blockchain/score.h>
#include <xpxchaincpp/model/blockchain/height.h>
#include <xpxchaincpp/model/blockchain/storage_info.h>
#include <xpxchaincpp/model/blockchain/multiple_block.h>
#include <xpxchaincpp/model/mosaic/multiple_mosaic.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/account/account_info.h>
#include <xpxchaincpp/model/mosaic/mosaic_name.h>
#include <xpxchaincpp/model/mosaic/mosaic_names.h>
#include <xpxchaincpp/model/namespace/namespace_info.h>
#include <xpxchaincpp/model/namespace/namespace_name.h>
#include <xpxchaincpp/model/namespace/namespace_names.h>
#include <xpxchaincpp/model/namespace/multiple_namespace_info.h>
#include <xpxchaincpp/model/account/account_properties.h>
#include <xpxchaincpp/model/network/network_info.h>
#include <xpxchaincpp/model/transaction_simple/transaction.h>
#include <xpxchaincpp/model/transaction_simple/transaction_status.h>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>



#include <xpxchaincpp/exceptions.h>
#include <xpxchaincpp/model/account/multiple_account_info.h>
#include <xpxchaincpp/model/transaction_simple/transaction_container.h>


using namespace xpx_chain_sdk::transactions_info;

namespace xpx_chain_sdk { namespace  internal { namespace json {
    namespace dto {
        template<typename Object, typename ObjectDto>
        Object fromDto(const ObjectDto& dto);

        template<typename Object, typename ObjectDto>
        Object from_json(const std::string &jsonStr) {
            ObjectDto dto;

            auto result = Parser::Read(dto, jsonStr);
            if (!result) {
                XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
            }

            return fromDto<Object, ObjectDto>(dto);
        }

		AggregateTransaction aggregateTransactionFromJson(const std::string& jsonStr);

		TransactionContainer transactions_from_json(const std::string& jsonStr);

		std::shared_ptr<transactions_info::BasicTransaction> transaction_from_json(const std::string& jsonStr);

        template<>
		TransactionContainer fromDto<TransactionContainer, TransactionContainerDto>(const TransactionContainerDto& dto);

        template<>
        BlockMeta fromDto<BlockMeta, BlockMetaDto>(const BlockMetaDto &dto);

        template<>
        BlockData fromDto<BlockData, BlockDataDto>(const BlockDataDto &dto);

        template<>
        Block fromDto<Block, BlockDto>(const BlockDto &dto);

        template<>
        StorageInfo fromDto<StorageInfo, StorageInfoDto>(const StorageInfoDto &dto) ;

        template<>
        ScoreInfo fromDto<ScoreInfo, ScoreInfoDto>(const ScoreInfoDto &dto) ;

        template<>
        Height fromDto<Height, HeightDto>(const HeightDto& dto) ;

        template<>
        Mosaic fromDto<Mosaic, MosaicDto>(const MosaicDto& dto);

        template<>
        MosaicMeta fromDto<MosaicMeta, MosaicMetaDto>(const MosaicMetaDto& dto);

        template<>
        MosaicData fromDto<MosaicData, MosaicDataDto>(const MosaicDataDto& dto) ;

        template<>
        MosaicInfo fromDto<MosaicInfo, MosaicInfoDto>(const MosaicInfoDto &dto);

        template<>
        MultipleMosaicInfo fromDto<MultipleMosaicInfo, MultipleMosaicInfoDto>(const MultipleMosaicInfoDto& dto);

        template<>
        MosaicName fromDto<MosaicName, MosaicNameDto>(const MosaicNameDto& dto);

        template<>
        MosaicNames fromDto<MosaicNames, MosaicNamesDto>(const MosaicNamesDto& dto);

        template<>
        Alias fromDto<Alias, AliasDto>(const AliasDto& dto);

        template<>
        NamespaceMeta fromDto<NamespaceMeta, NamespaceMetaDto>(const NamespaceMetaDto& dto);

        template<>
        NamespaceData fromDto<NamespaceData, NamespaceDataDto>(const NamespaceDataDto& dto);

        template<>
        NamespaceInfo fromDto<NamespaceInfo, NamespaceInfoDto>(const NamespaceInfoDto& dto);

        template<>
        MultipleNamespaceInfo fromDto<MultipleNamespaceInfo, MultipleNamespaceDto>(const MultipleNamespaceDto& dto);

        template<>
        NamespaceName fromDto<NamespaceName, NamespaceNameDto>(const NamespaceNameDto& dto);

        template<>
        NamespaceNames fromDto<NamespaceNames, NamespaceNamesDto>(const NamespaceNamesDto& dto);

        template<>
        MultipleBlock fromDto<MultipleBlock, MultipleBlocksDto>(const MultipleBlocksDto& dto);

        template<>
        Property fromDto<Property, PropertyDto>(const PropertyDto& dto);

        template<>
        AccountProperties fromDto<AccountProperties, AccountPropertyDto>(const AccountPropertyDto& dto) ;

        template<>
        MultipleAccountProperty fromDto<MultipleAccountProperty, MultipleAccountPropertyDto>(const MultipleAccountPropertyDto& dto);

        template<>
        MultisigInfo fromDto<MultisigInfo, MultisigInfoDto> (const MultisigInfoDto& dto);

        template<>
        AccountInfo fromDto<AccountInfo, AccountInfoDto> (const AccountInfoDto& dto);

        template<>
        MultipleAccountInfo fromDto<MultipleAccountInfo, MultipleAccountInfoDto>(const MultipleAccountInfoDto & dto);

        template<>
        MultisigLevel fromDto<MultisigLevel, MultisigLevelDto>(const MultisigLevelDto& dto);

        template<>
        MultisigGraph fromDto<MultisigGraph, MultisigGraphDto>(const MultisigGraphDto& dto);

        template<>
        NetworkInfo fromDto<NetworkInfo, NetworkInfoDto> (const NetworkInfoDto& dto);

        /// Transaction Meta

        template<>
		TransactionInfo fromDto<TransactionInfo, TransactionInfoDto>(const TransactionInfoDto& dto);

		template<>
		TransactionStatus fromDto<TransactionStatus, TransactionStatusDto>(const TransactionStatusDto& dto);

		template<>
		MultipleTransactionInfo fromDto<MultipleTransactionInfo, MultipleTransactionInfoDto>(const MultipleTransactionInfoDto& dto);

		template<>
		MultipleTransactionStatus fromDto<MultipleTransactionStatus, MultipleTransactionStatusDto>(const MultipleTransactionStatusDto& dto);

        /// Transactions
        template<>
		Cosignature fromDto<Cosignature, CosignatureDto>(const CosignatureDto& dto);

		template<>
		CosignatoryModification fromDto<CosignatoryModification, CosignatoryModificationDto>(const CosignatoryModificationDto& dto);

		template<>
		AccountTransactionPropertyModification fromDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(const TransactionPropertyModificationDto& dto);

		template<>
		AccountMosaicPropertyModification fromDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(const MosaicPropertyModificationDto& dto);

		template<>
		AccountAddressPropertyModification fromDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(const AddressPropertyModificationDto& dto);

		template<>
		Transaction fromDto<Transaction, TransactionDto >(const TransactionDto & dto);

		template<>
		transactions_info::EmbeddedTransaction fromDto<transactions_info::EmbeddedTransaction, EmbeddedTransactionDto >(const EmbeddedTransactionDto & dto);

		template<>
		AggregateTransaction fromDto<AggregateTransaction, AggregateTransactionDto >(const AggregateTransactionDto & dto);

		template<>
		AccountLinkTransaction fromDto<AccountLinkTransaction, AccountLinkTransactionDto >(const AccountLinkTransactionDto & dto);

		template<>
		LockFundsTransaction fromDto<LockFundsTransaction, LockFundsTransactionDto >(const LockFundsTransactionDto & dto);

		template<>
		ModifyMultisigAccountTransaction fromDto<ModifyMultisigAccountTransaction, ModifyMultisigAccountTransactionDto >(const ModifyMultisigAccountTransactionDto & dto);

		template<>
		MosaicDefinitionTransaction fromDto<MosaicDefinitionTransaction, MosaicDefinitionTransactionDto >(const MosaicDefinitionTransactionDto & dto);

		template<>
		MosaicSupplyChangeTransaction fromDto<MosaicSupplyChangeTransaction, MosaicSupplyChangeTransactionDto >(const MosaicSupplyChangeTransactionDto & dto);


		template<>
		RegisterNamespaceTransaction fromDto<RegisterNamespaceTransaction, RegisterNamespaceTransactionDto >(const RegisterNamespaceTransactionDto & dto);

		template<>
		SecretLockTransaction fromDto<SecretLockTransaction, SecretLockTransactionDto >(const SecretLockTransactionDto & dto);

		template<>
		SecretProofTransaction fromDto<SecretProofTransaction, SecretProofTransactionDto >(const SecretProofTransactionDto & dto);
		template<>
		TransferTransaction fromDto<TransferTransaction, TransferTransactionDto >(const TransferTransactionDto & dto);

		template<>
		AliasTransactionBase fromDto<AliasTransactionBase, AliasTransactionBaseDto >(const AliasTransactionBaseDto & dto);

		template<>
		AddressAliasTransaction fromDto<AddressAliasTransaction, AddressAliasTransactionDto >(const AddressAliasTransactionDto & dto);

		template<>
		MosaicAliasTransaction fromDto<MosaicAliasTransaction, MosaicAliasTransactionDto >(const MosaicAliasTransactionDto & dto);

		/// Account Property Transactions
		template<>
		AccountTransactionPropertyTransaction fromDto<AccountTransactionPropertyTransaction, AccountTransactionPropertyTransactionDto >(const AccountTransactionPropertyTransactionDto & dto);

		template<>
		AccountMosaicPropertyTransaction fromDto<AccountMosaicPropertyTransaction, AccountMosaicPropertyTransactionDto >(const AccountMosaicPropertyTransactionDto & dto);

		template<>
		AccountAddressPropertyTransaction fromDto<AccountAddressPropertyTransaction, AccountAddressPropertyTransactionDto >(const AccountAddressPropertyTransactionDto & dto);

		/// Embedded Transactions

		template<>
		EmbeddedAccountLinkTransaction fromDto<EmbeddedAccountLinkTransaction, EmbeddedAccountLinkTransactionDto >(const EmbeddedAccountLinkTransactionDto & dto);

		template<>
		EmbeddedLockFundsTransaction fromDto<EmbeddedLockFundsTransaction, EmbeddedLockFundsTransactionDto >(const EmbeddedLockFundsTransactionDto & dto);

		template<>
		EmbeddedModifyMultisigAccountTransaction fromDto<EmbeddedModifyMultisigAccountTransaction, EmbeddedModifyMultisigAccountTransactionDto >(const EmbeddedModifyMultisigAccountTransactionDto & dto);

		template<>
		EmbeddedMosaicDefinitionTransaction fromDto<EmbeddedMosaicDefinitionTransaction, EmbeddedMosaicDefinitionTransactionDto >(const EmbeddedMosaicDefinitionTransactionDto & dto);

		template<>
		EmbeddedMosaicSupplyChangeTransaction fromDto<EmbeddedMosaicSupplyChangeTransaction, EmbeddedMosaicSupplyChangeTransactionDto >(const EmbeddedMosaicSupplyChangeTransactionDto & dto);

		template<>
		EmbeddedRegisterNamespaceTransaction fromDto<EmbeddedRegisterNamespaceTransaction, EmbeddedRegisterNamespaceTransactionDto >(const EmbeddedRegisterNamespaceTransactionDto & dto);

		template<>
		EmbeddedSecretLockTransaction fromDto<EmbeddedSecretLockTransaction, EmbeddedSecretLockTransactionDto >(const EmbeddedSecretLockTransactionDto & dto);

		template<>
		EmbeddedSecretProofTransaction fromDto<EmbeddedSecretProofTransaction, EmbeddedSecretProofTransactionDto >(const EmbeddedSecretProofTransactionDto & dto);

		template<>
		EmbeddedTransferTransaction fromDto<EmbeddedTransferTransaction, EmbeddedTransferTransactionDto >(const EmbeddedTransferTransactionDto & dto);

		template<>
		EmbeddedAliasTransactionBase fromDto<EmbeddedAliasTransactionBase, EmbeddedAliasTransactionBaseDto >(const EmbeddedAliasTransactionBaseDto & dto);

		template<>
		EmbeddedAddressAliasTransaction fromDto<EmbeddedAddressAliasTransaction, EmbeddedAddressAliasTransactionDto >(const EmbeddedAddressAliasTransactionDto & dto);

		template<>
		EmbeddedMosaicAliasTransaction fromDto<EmbeddedMosaicAliasTransaction, EmbeddedMosaicAliasTransactionDto >(const EmbeddedMosaicAliasTransactionDto & dto);

		/// Account Property Transactions
		template<>
		EmbeddedAccountTransactionPropertyTransaction fromDto<EmbeddedAccountTransactionPropertyTransaction, EmbeddedAccountTransactionPropertyTransactionDto >(const EmbeddedAccountTransactionPropertyTransactionDto & dto);

		template<>
		EmbeddedAccountMosaicPropertyTransaction fromDto<EmbeddedAccountMosaicPropertyTransaction, EmbeddedAccountMosaicPropertyTransactionDto >(const EmbeddedAccountMosaicPropertyTransactionDto & dto);

		template<>
		EmbeddedAccountAddressPropertyTransaction fromDto<EmbeddedAccountAddressPropertyTransaction, EmbeddedAccountAddressPropertyTransactionDto >(const EmbeddedAccountAddressPropertyTransactionDto & dto);
	}
}
}
}

