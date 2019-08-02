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
#include <infrastructure/json/dto/mosaic_dto.h>
#include <nemcpp/model/account/account_property.h>
#include <nemcpp/model/account/account_properties.h>

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

namespace xpx_sdk::internal::json::dto {

    using AccountInfoDto = VariadicStruct<
            Field<STR_LITERAL("address"), std::string>,
            Field<STR_LITERAL("addressHeight"), Uint64>,
            Field<STR_LITERAL("publicKey"), std::string>,
            Field<STR_LITERAL("publicKeyHeight"), Uint64>,
            Field<STR_LITERAL("mosaics"), MultipleMosaicDto > >;

    using MultisigInfoDto = VariadicStruct<
            Field<STR_LITERAL("account"), std::string>,
            Field<STR_LITERAL("accountAddress"), std::string>,
            Field<STR_LITERAL("minApproval"), uint64_t >,
            Field<STR_LITERAL("minRemoval"), uint64_t >,
            Field<STR_LITERAL("cosignatories"), std::vector<std::string> >,
            Field<STR_LITERAL("multisigAccounts"), std::vector<std::string> > >;

    using PropertyDto = VariadicStruct<
            Field<STR_LITERAL("propertyType"), uint8_t >,
            Field<STR_LITERAL("values"), std::vector<std::string> > >;

    using AccountPropertyDto = VariadicStruct<
            Field<STR_LITERAL("address"), std::string>,
            Field<STR_LITERAL("properties"), std::vector<PropertyDto> > > ;

    using MultipleAccountPropertyDto = std::vector<AccountPropertyDto>;

    using MultipleAccountInfoDto = std::vector<AccountInfoDto>;


}