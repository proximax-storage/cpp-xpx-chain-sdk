#include "lower_higher.h"
#include "height.h"
#include <nemcpp/client.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <infrastructure/json/parser.h>

#include <iostream>

using namespace nem2_sdk::internal::dto;
using nem2_sdk::internal::json::Parser;

HeightDto HeightDto::from_json(const std::string& jsonStr) {
    std::cout << ' ' << jsonStr << std::endl;
    HeightDtoT dto;
    Parser::Read(dto, jsonStr);
	auto heightDto = HeightDto::getDto(dto);
	std::cout << heightDto.height << std::endl;
	return heightDto;

}

HeightDto HeightDto::getDto(const HeightDtoT& dto) {
    HeightDto height = {
            dto.value<"height"_>()
    };
    return height;
}
