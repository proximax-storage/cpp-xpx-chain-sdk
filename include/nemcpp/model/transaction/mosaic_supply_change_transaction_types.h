
#pragma once

#include <cstdint>

namespace xpx_sdk {
	
	/// Direction of supply change.
	enum class MosaicSupplyChangeDirection: uint8_t {
		Decrease,
		Increase
	};
}
