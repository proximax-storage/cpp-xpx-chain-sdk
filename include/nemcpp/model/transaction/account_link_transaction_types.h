
#pragma once

#include <cstdint>

namespace xpx_sdk::difficult_transactions {
	
	/// Account link transaction action.
	enum class AccountLinkTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
