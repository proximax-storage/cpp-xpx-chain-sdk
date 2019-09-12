
#pragma once

#include <nemcpp/utils/buffer.h>
#include <nemcpp/model/transaction/aggregate_transaction.h>

using xpx_sdk::Transaction;
namespace xpx_sdk { namespace internal {
	
	uint32_t MakeTransactionFullVersion(uint32_t version, NetworkIdentifier networkId);
	uint32_t ExtractTransactionVersion(uint32_t fullVersion);
	NetworkIdentifier ExtractTransactionNetworkId(uint16_t fullVersion);

	RawBuffer GetTransactionSignedData(const Transaction* transaction);
	Hash256 CalculateTransactionHash(const Transaction* transaction);
}}
