/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "infrastructure/json/uint32.h"

namespace xpx_chain_sdk { namespace internal { namespace json {
	
	Uint32::Uint32(uint32_t value): value_(value)
	{ }
	
	Uint32::operator uint32_t&()
	{
		return value_;
	}
	
	Uint32::operator const uint32_t&() const
	{
		return value_;
	}
}}}
