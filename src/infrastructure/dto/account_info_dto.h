/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/model/account/account_info.h>
#include <infrastructure/json/uint64.h>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/dto/mosaic_dto.h>

#include <stdint.h>
#include <string>

namespace nem2_sdk::internal::dto {

    using AccountInfoDtoT = VariadicStruct<
            Field<STR_LITERAL("address"), std::string>,
            Field<STR_LITERAL("addressHeight"), Uint64>,
            Field<STR_LITERAL("publicKey"), std::string>,
            Field<STR_LITERAL("publicKeyHeight"), Uint64>,
            Field<STR_LITERAL("mosaics"), std::vector<MosaicDtoT> > >;


    class AccountInfoDto : public AccountInfo {
    public:
        static AccountInfo from_json(const std::string & jsonStr);
        static AccountInfo getFromDto(const AccountInfoDtoT& dto );
    };
}