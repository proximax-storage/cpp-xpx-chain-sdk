
#pragma once

#include <nemcpp/service/read_result.h>
#include <nemcpp/utils/buffer.h>
#include <nemcpp/model/transaction/transaction.h>

#include <vector>

namespace nem2_sdk {
	
	/// Converts \a transaction to binary representation.
	std::vector<uint8_t> TransactionToBinary(const Transaction* transaction);
	
	/// Creates transaction from binary \a data.
	/// \note Throws \c serialization_error if \a data does not represent valid transaction.
	TransactionPtr TransactionFromBinary(RawBuffer data);
	
	/// Creates transaction from binary \a data. Does not throw.
	ReadResult TryCreateTransactionFromBinary(RawBuffer data, TransactionPtr& transaction);
}
