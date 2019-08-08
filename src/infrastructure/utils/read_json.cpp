#include "read_json.h"

using namespace xpx_sdk;
using namespace xpx_sdk::internal::json;
using namespace xpx_sdk::internal::json::dto;

namespace xpx_sdk::internal::json::dto {


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
    Height fromDto<Height, HeightDto>(const HeightDto &dto) {
        Height height = {
                dto.value<"height"_>()
        };
        return height;
    }

    template<>
    Mosaic fromDto<Mosaic, MosaicDto>(const MosaicDto &dto) {
        return {dto.value<"id"_>(), dto.value<"amount"_>()};
    }

    template<>
    MosaicMeta fromDto<MosaicMeta, MosaicMetaDto>(const MosaicMetaDto &dto) {
        MosaicMeta mosaicMeta = {
                dto.value<"id"_>()
        };

        return mosaicMeta;
    }

    template<>
    MosaicData fromDto<MosaicData, MosaicDataDto>(const MosaicDataDto &dto) {
        MosaicData mosaicData;
        mosaicData.mosaicId = dto.value<"mosaicId"_>();
        mosaicData.amount = dto.value<"supply"_>();
        mosaicData.height = dto.value<"height"_>();
        mosaicData.owner = dto.value<"owner"_>();
        for (auto& property : dto.value<"properties"_>()) {
            xpx_sdk::MosaicProperty tmp = {MosaicPropertyId(property.value<"id"_>()), property.value<"value"_>()};
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
    MultipleMosaicInfo fromDto<MultipleMosaicInfo, MultipleMosaicInfoDto>(const MultipleMosaicInfoDto &dto) {
        MultipleMosaicInfo mmdto;

        for (auto &mosaicDto: dto) {
            MosaicInfo mosaic = fromDto<MosaicInfo, MosaicInfoDto>(mosaicDto);
            mmdto.mosaics.push_back(mosaic);
        }

        return mmdto;
    }

    template<>
    MosaicName fromDto<MosaicName, MosaicNameDto>(const MosaicNameDto &dto) {

        MosaicName mosaicName = {
                dto.value<"parentid"_>(),
                dto.value<"mosaicid"_>(),
                dto.value<"name"_>()
        };
        return mosaicName;
    }

    template<>
    MosaicNames fromDto<MosaicNames, MosaicNamesDto>(const MosaicNamesDto &dto) {
        MosaicNames names;

        for (auto &nameDto: dto) {
            MosaicName mosaic = fromDto<MosaicName, MosaicNameDto>(nameDto);
            names.names.push_back(mosaic);
        }

        return names;
    }

    template<>
    Alias fromDto<Alias, AliasDto>(const AliasDto &dto) {

        Alias alias = {
                dto.value<"type"_>(),
                dto.value<"mosaicId"_>(),
                dto.value<"address"_>()
        };
        return alias;
    }

    template<>
    NamespaceMeta fromDto<NamespaceMeta, NamespaceMetaDto>(const NamespaceMetaDto &dto) {
        NamespaceMeta namespaceMeta = {
                dto.value<"active"_>(),
                dto.value<"index"_>(),
                dto.value<"id"_>()
        };
        return namespaceMeta;
    }

    template<>
    NamespaceData fromDto<NamespaceData, NamespaceDataDto>(const NamespaceDataDto &dto) {
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
    NamespaceInfo fromDto<NamespaceInfo, NamespaceInfoDto>(const NamespaceInfoDto &dto) {
        NamespaceInfo namespaceInfo = {
                fromDto<NamespaceMeta, NamespaceMetaDto>(dto.value<"meta"_>()),
                fromDto<NamespaceData, NamespaceDataDto>(dto.value<"namespace"_>())
        };
        return namespaceInfo;
    }

    template<>
    MultipleNamespaceInfo fromDto<MultipleNamespaceInfo, MultipleNamespaceDto>(const MultipleNamespaceDto &dto) {
        MultipleNamespaceInfo namespaceInfos;
        for (auto &nInfosDto : dto) {
            NamespaceInfo namespaceInfo = fromDto<NamespaceInfo, NamespaceInfoDto>(nInfosDto);
            namespaceInfos.namespaceInfos.push_back(namespaceInfo);
        }
        return namespaceInfos;
    }


    template<>
    NamespaceName fromDto<NamespaceName, NamespaceNameDto>(const NamespaceNameDto &dto) {
        NamespaceName namespaceName = {
                dto.value<"name"_>()
        };
        return namespaceName;
    }

    template<>
    NamespaceNames fromDto<NamespaceNames, NamespaceNamesDto>(const NamespaceNamesDto &dto) {
        NamespaceNames names;

        for (auto &nameT : dto) {
            names.names.push_back(fromDto<NamespaceName, NamespaceNameDto>(nameT));
        }
        return names;
    }

    template<>
    MultipleBlock fromDto<MultipleBlock, MultipleBlocksDto>(const MultipleBlocksDto &dto) {
        MultipleBlock mbdto;

        for (auto &blockDto: dto) {
            Block block = fromDto<Block, BlockDto>(blockDto);
            mbdto.blocks.push_back(block);
        }

        return mbdto;
    }

    // Accounts

    template<>
    Property fromDto<Property, PropertyDto>(const PropertyDto& dto) {
        Property property = {
                dto.value<"propertyType"_>(),
                dto.value<"values"_>()
        };
        return property;
    }

    template<>
    AccountProperty fromDto<AccountProperty, AccountPropertyDto>(const AccountPropertyDto& dto) {
        AccountProperty accountProperty;
        accountProperty.address = dto.value<"address"_>();
        for(auto & pdto : dto.value<"properties"_>()) {
            accountProperty.properties.push_back(fromDto<Property, PropertyDto>(pdto));
        }
        return accountProperty;
    }

    template<>
    MultipleAccountProperty fromDto<MultipleAccountProperty, MultipleAccountPropertyDto>(const MultipleAccountPropertyDto& dto) {
        MultipleAccountProperty multipleAccountProperty;
        for(auto & apdto : dto ){
            multipleAccountProperty.accountProperties.push_back(fromDto<AccountProperty, AccountPropertyDto>(apdto));
        }
        return multipleAccountProperty;
    }

    template<>
    MultisigInfo fromDto<MultisigInfo, MultisigInfoDto> (const MultisigInfoDto& dto) {
        MultisigInfo multisigInfo = {
                dto.value<"account"_>(),
                dto.value<"accountAddress"_>(),
                dto.value<"minApproval"_>(),
                dto.value<"minRemoval"_>(),
                dto.value<"cosignatories"_>(),
                dto.value<"multisigAccounts"_>()
        };
        return multisigInfo;
    }

    template<>
    AccountInfo fromDto<AccountInfo, AccountInfoDto> (const AccountInfoDto& dto) {
        AccountInfo accountInfo;
		auto accountDto = dto.value<"account"_>();
        accountInfo.address =  accountDto.value<"address"_>();
        accountInfo.addressHeight = accountDto.value<"addressHeight"_>();
        accountInfo.publicKey = accountDto.value<"publicKey"_>();
        accountInfo.publicKeyHeight = accountDto.value<"publicKeyHeight"_>();
        for(auto& mosaicDto : accountDto.value<"mosaics"_>()) {
            accountInfo.mosaics.push_back(fromDto<Mosaic, MosaicDto>(mosaicDto));
        }

        return accountInfo;
    }

	template<>
	MultipleAccountInfo fromDto<MultipleAccountInfo, MultipleAccountInfoDto>(const MultipleAccountInfoDto & dto) {
		MultipleAccountInfo multipleAccountInfo;
		for(auto & accountInfoDto : dto) {
			multipleAccountInfo.accountInfos.push_back(fromDto<AccountInfo, AccountInfoDto>(accountInfoDto));
		}
		return multipleAccountInfo;
	}

    template<>
    MultisigLevel fromDto<MultisigLevel, MultisigLevelDto>(const MultisigLevelDto& dto) {
        MultisigLevel multisigLevel;
        multisigLevel.level = dto.value<"level"_>();
        for(auto& multisig : dto.value<"multisigEntries"_>()) {
            multisigLevel.multisigEntries.push_back(fromDto<MultisigInfo, MultisigInfoDto>(multisig));
        }
        return multisigLevel;
    }

    template<>
    MultisigGraph fromDto<MultisigGraph, MultisigGraphDto>(const MultisigGraphDto& dto) {
        MultisigGraph multisigGraph;
        for(auto& multisigLevel : dto) {
            multisigGraph.multisigLevels.push_back(fromDto<MultisigLevel, MultisigLevelDto>(multisigLevel));
        }
        return multisigGraph;
    }

    //Network
    template<>
    NetworkInfo fromDto<NetworkInfo, NetworkInfoDto> (const NetworkInfoDto& dto) {
        NetworkInfo networkInfo = {
                dto.value<"name"_>(),
                dto.value<"description"_>()
        };
        return networkInfo;
    }
}