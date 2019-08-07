
#pragma once

#include <cstdint>

namespace xpx_sdk::simple_transactions {
	
	/// Direction of supply change.
	enum class MosaicSupplyChangeDirection: uint8_t {
		Decrease,
		Increase
	};
}
