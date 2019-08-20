#pragma once

#include <string>
#include <vector>

namespace xpx_sdk::simple_transactions {

	class TransactionInfo {
	public:
		std::string something;
	};

	class MultipleTransactionInfo {
	public:
		std::vector<TransactionInfo> infos;
	};
}