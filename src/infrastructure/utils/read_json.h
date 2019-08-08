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

#include <nemcpp/model/blockchain/block.h>
#include <nemcpp/model/blockchain/score.h>
#include <nemcpp/model/blockchain/height.h>
#include <nemcpp/model/blockchain/storage_info.h>
#include <nemcpp/model/blockchain/multiple_block.h>
#include <nemcpp/model/mosaic/multiple_mosaic.h>
#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/account/account_info.h>
#include <nemcpp/model/mosaic/mosaic_name.h>
#include <nemcpp/model/mosaic/mosaic_names.h>
#include <nemcpp/model/namespace/namespace_info.h>
#include <nemcpp/model/namespace/namespace_name.h>
#include <nemcpp/model/namespace/namespace_names.h>
#include <nemcpp/model/namespace/multiple_namespace_info.h>
#include <nemcpp/model/account/account_properties.h>
#include <nemcpp/model/network/network_info.h>
#include <nemcpp/model/transaction_simple/transaction.h>


#include <nemcpp/exceptions.h>
#include <nemcpp/model/account/multiple_account_info.h>
#include <iostream>


namespace xpx_sdk { namespace  internal { namespace json {
    namespace dto {
        template<typename Object, typename ObjectDto>
        Object fromDto(const ObjectDto& dto);

        template<typename Object, typename ObjectDto>
        Object from_json(const std::string &jsonStr) {
        	std::cout << jsonStr << std::endl;
            ObjectDto dto;

            auto result = Parser::Read(dto, jsonStr);
            if (!result) {
                NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
            }
            return fromDto<Object, ObjectDto>(dto);
        }

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
        AccountProperty fromDto<AccountProperty, AccountPropertyDto>(const AccountPropertyDto& dto) ;

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

        //Transactions

//		template<>
//		Transaction fromDto<Transaction, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			Transaction transaction;
//			transaction.size = dto.value<"size"_>();
//			transaction.signature = dto.value<"signature"_>();
//			transaction.signer = dto.value<"signer"_>();
//			transaction.version = dto.value<"version"_>();
//			transaction.type = dto.value<"type"_>();
//			transaction.maxFee = dto.value<"maxFee"_>();
//			transaction.deadline = dto.value<"deadline"_>();
//			return transaction;
//		}
//
//		template<>
//		EmbeddedTransaction fromDto<EmbeddedTransaction, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			EmbeddedTransaction transaction;
//			transaction.size = dto.value<"size"_>();
//			transaction.signer = dto.value<"signer"_>();
//			transaction.version = dto.value<"version"_>();
//			transaction.type = dto.value<"type"_>();
//			return transaction;
//		}
//
//		template<>
//		AggregateTransaction fromDto<AggregateTransaction, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			AggregateTransaction transaction(fromDto<Transaction, GeneralTransactionDto>(dto));
//			transaction.payloadSize = dto.value<"payloadSize"_>();
//			transaction.payload = dto.value<"payload"_>();
//			transaction.cosignatures = dto.value<"cosignatures"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TAccountLinkTransaction<TBase> fromDto<TAccountLinkTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TAccountLinkTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//			transaction.linkAction = dto.value<"linkAction"_>();
//			transaction.remoteAccountKey = dto.value<"remoteAccountKey"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TLockFundsTransaction<TBase> fromDto<TLockFundsTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TAccountLinkTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//			transaction.lockedMosaic = dto.value<"lockedMosaic"_>();
//			transaction.lockDuration = dto.value<"lockDuration"_>();
//			transaction.lockHash = dto.value<"lockHash"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TModifyMultisigAccountTransaction<TBase> fromDto<TModifyMultisigAccountTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TModifyMultisigAccountTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//			transaction.minRemovalDelta = dto.value<"minRemovalDelta"_>();
//			transaction.minApprovalDelta = dto.value<"minApprovalDelta"_>();
//			transaction.modificationsCount = dto.value<"modificationsCount"_>();
//			transaction.modifications = dto.value<"modifications"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TMosaicDefinitionTransaction<TBase> fromDto<TMosaicDefinitionTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TMosaicDefinitionTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//
//			transaction.nonce = dto.value<"nonce"_>();
//			transaction.mosaicId = dto.value<"mosaicId"_>();
//			transaction.optionalPropertiesCount = dto.value<"optionalPropertiesCount"_>();
//			transaction.flags = dto.value<"flags"_>();
//			transaction.divisibility = dto.value<"divisibility"_>();
//			for(auto & mosaicProperty : dto.value<"optionalProperties"_>()) {
//				transaction.optionalProperties.push_back(fromDto<MosaicProperty, MosaicPropertyDto>(mosaicProperty)) ;
//			}
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TMosaicSupplyChangeTransaction<TBase> fromDto<TMosaicSupplyChangeTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TMosaicSupplyChangeTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//			transaction.mosaicId = dto.value<"mosaicId"_>();
//			transaction.direction = dto.value<"direction"_>();
//			transaction.delta = dto.value<"delta"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TRegisterNamespaceTransaction<TBase> fromDto<TRegisterNamespaceTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TRegisterNamespaceTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//
//			transaction.namespaceType = dto.value<"namespaceType"_>();
//			transaction.durationOrParentId = dto.value<"durationOrParentId"_>();
//			transaction.namespaceId = dto.value<"namespaceId"_>();
//			transaction.namespaceNameSize = dto.value<"namespaceNameSize"_>();
//			transaction.namespaceName = dto.value<"namespaceName"_>();
//
//			return transaction;
//		}
//
//		template<typename TBase>
//		TSecretLockTransaction<TBase> fromDto<TSecretLockTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TSecretLockTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//
//			transaction.mosaicId = dto.value<"mosaicId"_>();
//			transaction.amount = dto.value<"amount"_>();
//			transaction.duration = dto.value<"duration"_>();
//			transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
//			transaction.secret = dto.value<"secret"_>();
//			transaction.recipient = dto.value<"recipient"_>();
//
//
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TSecretProofTransaction<TBase> fromDto<TSecretProofTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TSecretProofTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//
//			transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
//			transaction.secret = dto.value<"secret"_>();
//			transaction.proofSize = dto.value<"proofSize"_>();
//			transaction.proof = dto.value<"proof"_>();
//
//
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TTransferTransaction<TBase> fromDto<TTransferTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TTransferTransaction<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//
//			transaction.recipient = dto.value<"recipient"_>();
//			transaction.messageSize = dto.value<"messageSize"_>();
//			transaction.mosaicsCount = dto.value<"mosaicsCount"_>();
//			transaction.message = dto.value<"message"_>();
//			transaction.mosaics = dto.value<"mosaics"_>();
//
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TAliasTransactionBase<TBase> fromDto<TAliasTransactionBase<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TAliasTransactionBase<TBase> transaction(fromDto<TBase, GeneralTransactionDto>(dto));
//
//			transaction.aliasAction = dto.value<"aliasAction"_>();
//			transaction.namespaceId = dto.value<"namespaceId"_>();
//
//			return transaction;
//		}
//
//
//
//		template<typename TBase>
//		TAddressAliasTransaction<TBase> fromDto<TAddressAliasTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TAddressAliasTransaction<TBase> transaction(fromDto<TAliasTransactionBase<TBase>, GeneralTransactionDto>(dto));
//			transaction.address = dto.value<"address"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TMosaicAliasTransaction<TBase> fromDto<TMosaicAliasTransaction<TBase>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TAddressAliasTransaction<TBase> transaction(fromDto<TAliasTransactionBase<TBase>, GeneralTransactionDto>(dto));
//			transaction.mosaicId = dto.value<"mosaicId"_>();
//			return transaction;
//		}
//
//
//		template<typename TBase>
//		TAccountPropertyTransaction<TBase> fromDto<TAccountPropertyTransaction<TBase, T>, GeneralTransactionDto >(const GeneralTransactionDto & dto) {
//			TAccountPropertyTransaction<TBase, T> transaction(fromDto<TBase>, GeneralTransactionDto>(dto));
//
//			transaction.propertyType = dto.value<"propertyType"_>();
//			transaction.modificationsCount = dto.value<"modificationsCount"_>();
//			transaction.modifications = dto.value<"modifications"_>(); //??
//
//			return transaction;
//		}

	}
}
}
}

