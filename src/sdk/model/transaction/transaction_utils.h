/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>
#include <xpxchaincpp/model/transaction/aggregate_transaction.h>

using xpx_chain_sdk::Transaction;
namespace xpx_chain_sdk { namespace internal {
	
	uint32_t MakeTransactionFullVersion(uint32_t version, NetworkIdentifier networkId);
	uint32_t ExtractTransactionVersion(uint32_t fullVersion);
	NetworkIdentifier ExtractTransactionNetworkId(uint16_t fullVersion);

	RawBuffer GetTransactionSignedData(const Transaction* transaction);
	Hash256 CalculateTransactionHash(const Transaction* transaction);
}}
