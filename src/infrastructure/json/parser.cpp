
#include "infrastructure/json/parser.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace nem2_sdk { namespace internal { namespace json {
	
	ParseResult::ParseResult(bool result, std::string_view invalidField):
		result_(result),
		invalidField_(invalidField)
	{ }
	
	const std::string& ParseResult::invalidField() const
	{
		return invalidField_;
	}
	
	ParseResult::operator bool() const
	{
		return result_;
	}
	
	bool Parser::setData(std::string_view data)
	{
		document_.Parse(data.data(), data.size());
		return !document_.HasParseError();
	}
	
	std::string Parser::getData(OutputMode mode) const
	{
		using namespace rapidjson;
		
		StringBuffer buffer;
		
		if (mode == OutputMode::Pretty) {
			PrettyWriter<StringBuffer> writer(buffer);
			document_.Accept(writer);
		} else {
			Writer<StringBuffer> writer(buffer);
			document_.Accept(writer);
		}
		
		return buffer.GetString();
	}
}}}
