
#pragma once

#include <nemcpp/service/read_result.h>
#include <nemcpp/utils/buffer.h>
#include <nemcpp/model/transaction/transaction.h>

#include <string>
#include <vector>
using xpx_sdk::complex_transactions::TransactionPtr;
using xpx_sdk::complex_transactions::Transaction;
namespace xpx_sdk {
	
	/// Converts \a transaction to binary representation.
	std::vector<uint8_t> TransactionToBinary(const Transaction* transaction);
	
	/// Creates transaction from binary \a data.
	/// \note Throws \c serialization_error if \a data does not represent valid transaction.
	TransactionPtr TransactionFromBinary(RawBuffer data);
	
	/// Creates transaction from binary \a data. Does not throw.
	ReadResult TryCreateTransactionFromBinary(RawBuffer data, TransactionPtr& transaction);
	
	/// Converts \a transaction to json representation.
	std::string TransactionToJson(const Transaction* transaction);
	
	/// Creates transaction from binary \a data.
	/// \note Throws \c serialization_error if \a data does not represent valid transaction.
	/// \note This function expects that \a data contains complete json document.
	TransactionPtr TransactionFromJson(std::string_view data);
	
	/// Creates transaction from json \a data. Does not throw.
	/// \note This function expects that \a data contains complete json document.
	ReadResult TryCreateTransactionFromJson(std::string_view data, TransactionPtr& transaction);
}
