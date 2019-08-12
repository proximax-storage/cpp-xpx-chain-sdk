
#pragma once

#include <cstdint>

namespace xpx_sdk::complex_transactions {
	
	/// Direction of supply change.
	enum class MosaicSupplyChangeDirection: uint8_t {
		Decrease,
		Increase
	};
}
