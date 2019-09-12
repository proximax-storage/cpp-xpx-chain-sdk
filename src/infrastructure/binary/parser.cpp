/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "infrastructure/binary/parser.h"

namespace xpx_chain_sdk { namespace internal { namespace binary {
	
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
