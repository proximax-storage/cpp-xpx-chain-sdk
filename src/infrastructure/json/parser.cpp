/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "infrastructure/json/parser.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace xpx_chain_sdk { namespace internal { namespace json {
	
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
