#pragma once

#include <nemcpp/model/blockchain/block.h>

#include <boost/property_tree/ptree.hpp>
#include "infrastructure/utils/variadic_struct.h"
#include "infrastructure/json/uint64.h"
#include <stdint.h>

using Uint64 = nem2_sdk::internal::json::Uint64;


namespace nem2_sdk::internal::dto {

    using BlockMetaDtoT = VariadicStruct<
    Field<STR_LITERAL("hash"), std::string>,
    Field<STR_LITERAL("generationHash"), std::string>,
    Field<STR_LITERAL("totalFee"), Uint64>,
    Field<STR_LITERAL("numTransactions"), Uint64> >;

    using BlockDataDtoT = VariadicStruct<
            Field<STR_LITERAL("signature"), std::string>,
            Field<STR_LITERAL("timestamp"), Uint64 >,
            Field<STR_LITERAL("difficulty"), Uint64 >,
            Field<STR_LITERAL("feeMultiplier"), Uint64 >,
            Field<STR_LITERAL("previousBlockHash"), std::string>,
            Field<STR_LITERAL("blockTransactionsHash"), std::string>,
            Field<STR_LITERAL("blockReceiptsHash"), std::string>,
            Field<STR_LITERAL("stateHash"), std::string>,
            Field<STR_LITERAL("beneficiaryPublicKey"), std::string> >;



	class BlockMetaDto : public BlockMeta {
	public:
		static BlockMetaDto from_json(std::istream& json);
		static BlockMetaDto getDto(const BlockMetaDtoT& dto);
	};

	class BlockDataDto : public BlockData {
	public:
		static BlockDataDto from_json(std::istream& json);
		static BlockDataDto getDto(const BlockDataDtoT& dto);
//		static BlockDataDto from_ptree(boost::property_tree::ptree &pTree);
	};

    using BlockDtoT = VariadicStruct<
            Field<STR_LITERAL("meta"), BlockMetaDtoT>,
            Field<STR_LITERAL("data"), BlockDataDtoT > >;

	class BlockDto : public Block {
	public:
//	    static BlockDto from_json_custom(std::istream& json);
		static BlockDto from_json(std::istream& json);
        static BlockDto getDto(const BlockDtoT &dto);
//		static BlockDto from_ptree(boost::property_tree::ptree &pTree);
	};
}
