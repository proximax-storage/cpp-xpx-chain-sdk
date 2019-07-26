/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "height.h"
#include <infrastructure/json/parser.h>
#include <nemcpp/exceptions.h>
#include <iostream>

using namespace nem2_sdk::internal::dto;
using nem2_sdk::internal::json::Parser;

HeightDto HeightDto::from_json(const std::string& jsonStr) {
    HeightDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }
	HeightDto heightDto = HeightDto::getFromDto(dto);
	return heightDto;

}

HeightDto HeightDto::getFromDto(const HeightDtoT& dto) {
    HeightDto height = {
            dto.value<"height"_>()
    };
    return height;
}
