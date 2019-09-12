/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk { namespace internal { namespace json {
	
	class Uint64 {
	public:
	    Uint64() {value_=0;}
		Uint64(uint64_t value);
		operator uint64_t&();
		operator const uint64_t&() const;
		
	private:
		uint64_t value_;
	};
}}}
