#pragma once

#include <string>
#include <vector>

namespace xpx_sdk::simple_transactions {
	class TransactionStatus {
	public:
		std::string group, status, hash;
		uint64_t deadline, height;
	};

	class MultipleTransactionStatus {
	public:
		std::vector<TransactionStatus> statuses;
	};
}