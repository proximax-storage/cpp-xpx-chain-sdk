
#include "infrastructure/binary/parser.h"

namespace xpx_sdk { namespace internal { namespace binary {
	
	ParseResult::ParseResult(bool result, size_t processedSize):
		result_(result),
		processedSize_(processedSize)
	{ }
	
	size_t ParseResult::processedSize() const
	{
		return processedSize_;
	}
	
	ParseResult::operator bool() const
	{
		return result_;
	}
}}}
