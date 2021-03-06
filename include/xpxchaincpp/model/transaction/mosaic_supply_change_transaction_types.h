/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {
	
	/// Direction of supply change.
	enum class MosaicSupplyChangeDirection: uint8_t {
		Decrease,
		Increase
	};
}
