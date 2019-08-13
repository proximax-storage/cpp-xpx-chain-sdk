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


namespace xpx_sdk { namespace  internal { namespace json {
    namespace dto {
        template<typename Object, typename ObjectDto>
        Object fromDto(const ObjectDto& dto);

        template<typename Object, typename ObjectDto>
        Object from_json(const std::string &jsonStr) {
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

	}
}
}
}

