/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>

#include <string>
#include <stdint.h>
#include <array>
#include <vector>

namespace nem2_sdk::internal::dto {
    using nem2_sdk::internal::json::Uint64;

    using MosaicPropertyDtoT = std::array<uint64_t , 2>;

    using TransactionMetaDtoT = VariadicStruct<
            Field<STR_LITERAL("height"), Uint64>,
            Field<STR_LITERAL("hash"), std::string>,
            Field<STR_LITERAL("merkleComponentHash"), std::string>,
            Field<STR_LITERAL("index"), uint64_t >,
            Field<STR_LITERAL("id"), std::string> >;


    using GeneraltransactionDtoT = VariadicStruct<
            Field<STR_LITERAL("signature"), std::string>,
            Field<STR_LITERAL("signer"), std::string>,
            Field<STR_LITERAL("version"), uint64_t >,
            Field<STR_LITERAL("type"), uint64_t >,
            Field<STR_LITERAL("max_fee"), Uint64 >,
            Field<STR_LITERAL("deadline"), Uint64 >,
            Field<STR_LITERAL("mosaicNonce"), uint64_t >,
            Field<STR_LITERAL("mosaicId"), Uint64 >,
            Field<STR_LITERAL("properties"), std::vector<MosaicPropertyDtoT> >,
            Field<STR_LITERAL("namespaceId"), uint64_t>,
            Field<STR_LITERAL("aliasAction"), bool>,
            Field<STR_LITERAL("namespaceType"), bool>,
            Field<STR_LITERAL("duration"), uint64_t >,
            Field<STR_LITERAL("parentId"), uint64_t>,
            Field<STR_LITERAL("name"), std::string >,
            Field<STR_LITERAL("address"), std::string> >;

}

