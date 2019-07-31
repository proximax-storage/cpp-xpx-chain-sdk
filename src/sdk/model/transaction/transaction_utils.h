
#pragma once

#include <nemcpp/utils/buffer.h>
#include <nemcpp/model/transaction/aggregate_transaction.h>

namespace xpx_sdk { namespace internal {
	
	uint16_t MakeTransactionFullVersion(uint8_t version, NetworkIdentifier networkId);
	uint8_t ExtractTransactionVersion(uint16_t fullVersion);
	NetworkIdentifier ExtractTransactionNetworkId(uint16_t fullVersion);
	
	RawBuffer GetTransactionSignedData(const Transaction* transaction);
	Hash256 CalculateTransactionHash(const Transaction* transaction);
}}
