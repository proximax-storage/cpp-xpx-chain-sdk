#include <infrastructure/json/parser.h>


namespace nem2_sdk::internal::json {
    template<typename Object, typename ObjectDto, typename ObjectExtractor>
    Object from_json(const std::string& jsonStr) {
        ObjectDto dto;
        auto result = Parser::Read(dot, jsonStr);
        if(!result) {
            NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
        }
        return ObjectExtractor::fromDto(dto);
    }
}
