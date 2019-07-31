#pragma once

#include <infrastructure/json/parser.h>
#include <infrastructure/json/dto/block_dto.h>
#include <infrastructure/json/dto/mosaic_dto.h>
#include <infrastructure/json/dto/namespace_dto.h>
#include <infrastructure/json/dto/height.h>
#include <infrastructure/json/dto/account_info_dto.h>
#include <infrastructure/json/dto/multiple_namespace_dto.h>
#include <infrastructure/json/dto/multiple_blocks_dto.h>
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
#include <nemcpp/exceptions.h>



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
        BlockMeta fromDto<BlockMeta, BlockMetaDto>(const BlockMetaDto &dto) {
            BlockMeta meta;
            meta.hash = dto.value<"hash"_>();
            meta.generationHash = dto.value<"generationHash"_>();
            meta.totalFee = dto.value<"totalFee"_>();  // note how Uint64 type from dto ([high, low] in json automatically converted to uint64_t
            meta.numTransactions = dto.value<"numTransactions"_>();
            return meta;
        }

        template<>
        BlockData fromDto<BlockData, BlockDataDto>(const BlockDataDto &dto) {
            BlockData data = {
                    dto.value<"signature"_>(),
                    dto.value<"timestamp"_>(),
                    dto.value<"difficulty"_>(),
                    dto.value<"feeMultiplier"_>(),
                    dto.value<"previousBlockHash"_>(),
                    dto.value<"blockTransactionsHash"_>(),
                    dto.value<"blockReceiptsHash"_>(),
                    dto.value<"stateHash"_>(),
                    dto.value<"beneficiary"_>()
            };
            return data;
        }

        template<>
        Block fromDto<Block, BlockDto>(const BlockDto &dto) {
            Block block = {
                    fromDto<BlockMeta, BlockMetaDto>(dto.value<"meta"_>()),
                    fromDto<BlockData, BlockDataDto>(dto.value<"block"_>())
            };
            return block;
        }

        template<>
        StorageInfo fromDto<StorageInfo, StorageInfoDto>(const StorageInfoDto &dto) {

            StorageInfo storageInfo = {
                    dto.value<"numBlocks"_>(),
                    dto.value<"numTransactions"_>(),
                    dto.value<"numAccounts"_>()
            };

            return storageInfo;
        }

        template<>
        ScoreInfo fromDto<ScoreInfo, ScoreInfoDto>(const ScoreInfoDto &dto) {
            ScoreInfo scoreInfo = {
                    dto.value<"scoreHigh"_>(),
                    dto.value<"scoreLow"_>()
            };

            return scoreInfo;
        }

        template<>
        Height fromDto<Height, HeightDto>(const HeightDto& dto) {
            Height height = {
                    dto.value<"height"_>()
            };
            return height;
        }

        template<>
        Mosaic fromDto<Mosaic, MosaicDto>(const MosaicDto& dto) {
            return {dto.value<"id"_>(), dto.value<"amount"_>()};
        }

        template<>
        AccountInfo fromDto<AccountInfo, AccountInfoDto>(const AccountInfoDto& dto ) {
            AccountInfo accountInfo;
            accountInfo.address = dto.value<"address"_>();
            accountInfo.addressHeight = dto.value<"addressHeight"_>();
            accountInfo.publicKey = dto.value<"publicKey"_>();
            accountInfo.publicKeyHeight = dto.value<"publicKeyHeight"_>();

            for(auto& mosaicDto : dto.value<"mosaics"_>()) {
                accountInfo.mosaics.push_back(fromDto<Mosaic, MosaicDto>(mosaicDto));
            }

            return accountInfo;

        }



        template<>
        MosaicMeta fromDto<MosaicMeta, MosaicMetaDto>(const MosaicMetaDto& dto) {
            MosaicMeta mosaicMeta = {
                    dto.value<"active"_>(),
                    dto.value<"index"_>(),
                    dto.value<"id"_>()
            };

            return mosaicMeta;
        }

        template<>
        MosaicData fromDto<MosaicData, MosaicDataDto>(const MosaicDataDto& dto) {
            MosaicData mosaicData;
            mosaicData.mosaicId = dto.value<"mosaicid"_>();
            mosaicData.amount = dto.value<"supply"_>();
            mosaicData.height = dto.value<"height"_>();
            mosaicData.owner = dto.value<"owner"_>();
            for(auto property : dto.value<"properties"_>()) {
                xpx_sdk::MosaicProperty tmp = {MosaicPropertyId(property[0]), property[1]};
                mosaicData.properties.push_back(tmp);
            }
            return mosaicData;
        }

        template<>
        MosaicInfo fromDto<MosaicInfo, MosaicInfoDto>(const MosaicInfoDto &dto) {
            MosaicInfo mosaicInfo = {
                    fromDto<MosaicMeta, MosaicMetaDto>(dto.value<"meta"_>()),
                    fromDto<MosaicData, MosaicDataDto>(dto.value<"mosaic"_>())
            };
            return mosaicInfo;
        }

        template<>
        MultipleMosaicInfo fromDto<MultipleMosaicInfo, MultipleMosaicDto>(const MultipleMosaicDto& dto) {
            MultipleMosaicInfo mmdto;

            for (auto& mosaicDto: dto) {
                MosaicInfo mosaic = fromDto<MosaicInfo, MosaicInfoDto>(mosaicDto);
                mmdto.mosaics.push_back(mosaic);
            }

            return mmdto;
        }

        template<>
        MosaicName fromDto<MosaicName, MosaicNameDto>(const MosaicNameDto& dto) {

            MosaicName mosaicName =  {
                    dto.value<"parentid"_>(),
                    dto.value<"mosaicid"_>(),
                    dto.value<"name"_>()
            };
            return mosaicName;
        }

        template<>
        MosaicNames fromDto<MosaicNames, MosaicNamesDto>(const MosaicNamesDto& dto) {
            MosaicNames names;

            for (auto& nameDto: dto) {
                MosaicName mosaic = fromDto<MosaicName, MosaicNameDto>(nameDto);
                names.names.push_back(mosaic);
            }

            return names;
        }

        template<>
        Alias fromDto<Alias, AliasDto>(const AliasDto& dto) {

            Alias alias = {
                    dto.value<"type"_>(),
                    dto.value<"mosaicId"_>(),
                    dto.value<"address"_>()
            };
            return alias;
        }

        template<>
        NamespaceMeta fromDto<NamespaceMeta, NamespaceMetaDto>(const NamespaceMetaDto& dto) {
            NamespaceMeta namespaceMeta = {
                    dto.value<"active"_>(),
                    dto.value<"index"_>(),
                    dto.value<"id"_>()
            };
            return namespaceMeta;
        }

        template<>
        NamespaceData fromDto<NamespaceData, NamespaceDataDto>(const NamespaceDataDto& dto) {
            NamespaceData namespaceData = {
                    dto.value<"owner"_>(),
                    dto.value<"ownerAddress"_>(),
                    dto.value<"startHeight"_>(),
                    dto.value<"endHeight"_>(),
                    dto.value<"depth"_>(),
                    dto.value<"level0"_>(),
                    dto.value<"level1"_>(),
                    dto.value<"level2"_>(),
                    dto.value<"type"_>(),
                    fromDto<Alias, AliasDto>(dto.value<"alias"_>()),
                    dto.value<"parentId"_>()
            };
            return namespaceData;
        }

        template<>
        NamespaceInfo fromDto<NamespaceInfo, NamespaceInfoDto>(const NamespaceInfoDto& dto) {
            NamespaceInfo namespaceInfo = {
                    fromDto<NamespaceMeta, NamespaceMetaDto>(dto.value<"meta"_>()),
                    fromDto<NamespaceData, NamespaceDataDto>(dto.value<"namespace"_>())
            };
            return namespaceInfo;
        }

        template<>
        MultipleNamespaceInfo fromDto<MultipleNamespaceInfo, MultipleNamespaceDto>(const MultipleNamespaceDto& dto) {
            MultipleNamespaceInfo namespaceInfos;
            for(auto & nInfosDto : dto) {
                NamespaceInfo namespaceInfo = fromDto<NamespaceInfo, NamespaceInfoDto>(nInfosDto);
                namespaceInfos.namespaceInfos.push_back(namespaceInfo);
            }
            return namespaceInfos;
        }


        template<>
        std::string fromDto<std::string, NamespaceNameDto>(const NamespaceNameDto& dto) {
            return dto.value<"name"_>();
        }

        template<>
        NamespaceNames fromDto<NamespaceNames, NamespaceNamesDto>(const NamespaceNamesDto& dto) {
            NamespaceNames names;

            for(auto& nameT : dto) {
                names.names.push_back(fromDto<NamespaceName, NamespaceNameDto>(nameT));
            }
            return names;
        }

        template<>
        MultipleBlock fromDto<MultipleBlock, MultipleBlocksDto>(const MultipleBlocksDto& dto) {
            MultipleBlock mbdto;

            for (auto& blockDto: dto) {
                Block block = fromDto<Block, BlockDto>(blockDto);
                mbdto.blocks.push_back(block);
            }

            return mbdto;
        }

    
    }
}
}
}

