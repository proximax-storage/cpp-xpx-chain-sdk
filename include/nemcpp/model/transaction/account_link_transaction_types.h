
#pragma once

#include <cstdint>

namespace nem2_sdk {
	
	/// Account link transaction action.
	enum class AccountLinkTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
