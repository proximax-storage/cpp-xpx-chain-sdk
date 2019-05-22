
#pragma once

#include <cstdint>

namespace nem2_sdk {
	
	/// Direction of supply change.
	enum class MosaicSupplyChangeDirection: uint8_t {
		Decrease,
		Increase
	};
}
