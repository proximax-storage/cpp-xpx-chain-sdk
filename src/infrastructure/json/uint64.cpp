/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "infrastructure/json/uint64.h"

namespace xpx_chain_sdk { namespace internal { namespace json {
	
	Uint64::Uint64(uint64_t value): value_(value)
	{ }
	
	Uint64::operator uint64_t&()
	{
		return value_;
	}
	
	Uint64::operator const uint64_t&() const
	{
		return value_;
	}
}}}
