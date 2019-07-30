/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/model/account/account_info.h>
#include <nemcpp/model/account/multisig_info.h>
#include <infrastructure/json/uint64.h>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/dto/mosaic_dto.h>

#include <stdint.h>
#include <string>

/*
 "multisig":

{

    "account": "string",
    "accountAddress": "string",
    "minApproval": 0,
    "minRemoval": 0,
    "cosignatories":

[

    "string"

],
"multisigAccounts":

    [
        "string"
    ]

}*/

namespace nem2_sdk::internal::dto {

    using AccountInfoDtoT = VariadicStruct<
            Field<STR_LITERAL("address"), std::string>,
            Field<STR_LITERAL("addressHeight"), Uint64>,
            Field<STR_LITERAL("publicKey"), std::string>,
            Field<STR_LITERAL("publicKeyHeight"), Uint64>,
            Field<STR_LITERAL("mosaics"), std::vector<MosaicDtoT> > >;

    using MultisigInfoDtoT = VariadicStruct<
            Field<STR_LITERAL("account"), std::string>,
            Field<STR_LITERAL("accountAddress"), std::string>,
            Field<STR_LITERAL("minApproval"), uint64_t >,
            Field<STR_LITERAL("minRemoval"), uint64_t >,
            Field<STR_LITERAL("cosignatories"), std::vector<std::string> >,
            Field<STR_LITERAL("multisigAccounts"), std::vector<std::string> > >;


    class AccountInfoDto {
    public:
        static AccountInfo from_json(const std::string & jsonStr);
        static AccountInfo getFromDto(const AccountInfoDtoT& dto );
    };

    class MultisigInfoDto {
    public:
        static MultisigInfo from_json(const std::string & jsonStr);
        static MultisigInfo getFromDto(const MultisigInfoDtoT& dto );
    };
}