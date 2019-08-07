
#pragma once

#include <cstdint>

namespace xpx_sdk::difficult_transactions {
	
	/// Direction of supply change.
	enum class MosaicSupplyChangeDirection: uint8_t {
		Decrease,
		Increase
	};
}
