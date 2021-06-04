/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk { namespace internal { namespace json {
	
	class Uint32 {
	public:
	    Uint32() {value_=0;}
		Uint32(uint32_t value);
		operator uint32_t&();
		operator const uint32_t&() const;
		
	private:
		uint32_t value_;
	};
}}}
