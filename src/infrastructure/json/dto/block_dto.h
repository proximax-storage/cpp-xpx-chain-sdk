/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include "infrastructure/utils/variadic_struct.h"
#include "infrastructure/json/uint64.h"
#include "infrastructure/json/descriptors.h"
#include <stdint.h>



// Json Example
/*{
    "meta": {
        "hash": "A142B49A7E8C2348B9179F21DFB35C68AA46806E1B521261F591F678CC4CC1B8",
        "generationHash": "43861500A6A14979A4473FC12D5D2EB19EBC2D85FBBA3D74EA077B69D3CA2D96",
        "totalFee": [0, 0],
        "subCacheMerkleRoots": ["AE1C770CFD3245CE47377DCCC9C3CB1E1533E452B3452390B00E5F1363CC2F54", "A316D3BA2593D7BA6866C5F7B2F0D66407622620A1431027DBBF7A39F38184F9", "7BEF761316E8B4C58BDB27F7797A11E28FFCFDEB15B8F64D7A646DB7361D9734", "8990BBF9AA8E7ACD27809BFB4F0F0C99C42A047028F5636CE20FB5975479330F", "78D5B456F17921AE4C09B0A5E3154FDB83D49A87D66D303EED299277B9D73923", "0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", "221E2CBBAA94778347FBDB478FA0631601993A5DC581FFC018D00EE927EF8020", "9BBD9B1D891A14044B625BAD08EE64594708BBFC73A8B1213E80C8185CDAF7B8", "2B48F78D16738A9A121CE108A66BA4874E407BF3A63AAB22FBE4AB161B490C55"],
        "numTransactions": 0,
        "numStatements": 1
    },
    "data": {
        "signature": "3264980230498E7BB0CEDD8E13CFD0DA92B456CA028A34D24FDE45B190315FE84FDADFB237E5F25F1DA0C0F3CDAC9FB5E3AFFF3C902FFCF7A872834B72664604",
        "signer": "D9A659A3AA42FD62BE88E1D96B0F10EB91F6097F8D24EC8FD7C94EC6455735EC",
        "version": 43011,
        "type": 33091,
        "height": [137105, 0],
        "timestamp": [1071392131, 24],
        "difficulty": [6276, 0],
        "feeMultiplier": 0,
        "previousBlockHash": "8ADF5FE1D9C27CFF29F4718EB1BF1C7262402FCD840AB69B3C04FF6AA9D3FFD8",
        "blockTransactionsHash": "0000000000000000000000000000000000000000000000000000000000000000",
        "blockReceiptsHash": "32BD8DDF594D265CCB6CC6349EEA26182A58414414D61E034DF23BC74350093D",
        "stateHash": "77D5311125173169FFD4E02EBDAA03866B0425970AB1E8F390BDDF52056A7744",
        "beneficiary": "0000000000000000000000000000000000000000000000000000000000000000",
        "feeInterest": 1,
        "feeInterestDenominator": 1
    }
}
 */

namespace xpx_sdk::internal::json::dto {

	using Uint64 = xpx_sdk::internal::json::Uint64;

    using BlockMetaDto = VariadicStruct<
            Field<STR_LITERAL("hash"), std::string>,
            Field<STR_LITERAL("generationHash"), std::string>,
            Field<STR_LITERAL("totalFee"), Uint64>,
            Field<STR_LITERAL("numTransactions"), uint64_t >,
            Field<STR_LITERAL("numStatements"), uint64_t, desc::Optional >>;

    using BlockDataDto = VariadicStruct<
            Field<STR_LITERAL("signature"), std::string>,
            Field<STR_LITERAL("timestamp"), Uint64 >,
            Field<STR_LITERAL("difficulty"), Uint64 >,
            Field<STR_LITERAL("feeMultiplier"), uint64_t >,
            Field<STR_LITERAL("previousBlockHash"), std::string>,
            Field<STR_LITERAL("blockTransactionsHash"), std::string>,
            Field<STR_LITERAL("blockReceiptsHash"), std::string>,
            Field<STR_LITERAL("stateHash"), std::string>,
            Field<STR_LITERAL("beneficiary"), std::string> >;

    using BlockDto = VariadicStruct<
            Field<STR_LITERAL("meta"), BlockMetaDto>,
            Field<STR_LITERAL("block"), BlockDataDto > >;

    using StorageInfoDto = VariadicStruct<
            Field<STR_LITERAL("numBlocks"), uint64_t>,
            Field<STR_LITERAL("numTransactions"), uint64_t>,
            Field<STR_LITERAL("numAccounts"), uint64_t> >;

    using ScoreInfoDto = VariadicStruct<
            Field<STR_LITERAL("scoreHigh"), Uint64>,
            Field<STR_LITERAL("scoreLow"), Uint64> >;

}
