
#pragma once

#include <cstdint>

namespace xpx_sdk::complex_transactions {
	
	/// Account link transaction action.
	enum class AccountLinkTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
