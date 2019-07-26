/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "account_info_dto.h"
#include <infrastructure/json/parser.h>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;

using nem2_sdk::internal::json::Parser;

AccountInfo AccountInfoDto::from_json(const std::string & jsonStr) {
    AccountInfoDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    AccountInfo accountInfo = AccountInfoDto::getFromDto(dto);
    return accountInfo;
}

AccountInfo AccountInfoDto::getFromDto(const AccountInfoDtoT& dto ) {
    AccountInfo accountInfo;
    accountInfo.address = dto.value<"address"_>();
    accountInfo.addressHeight = dto.value<"addressHeight"_>();
    accountInfo.publicKey = dto.value<"publicKey"_>();
    accountInfo.publicKeyHeight = dto.value<"publicKeyHeight"_>();

    for(auto& mosaicDtoT : dto.value<"mosaics"_>()) {
        accountInfo.mosaics.push_back(MosaicDto::getFromDto(mosaicDtoT));
    }

    return accountInfo;

}
