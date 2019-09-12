
#pragma once

#include <cstdint>

namespace xpx_sdk {
	
	/// Account link transaction action.
	enum class AccountLinkTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
