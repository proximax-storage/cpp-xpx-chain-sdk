
#include "sdk/model/transaction/transaction_utils.h"
#include "infrastructure/binary/offsets.h"

#include <xpxchaincpp/crypto/hash.h>
#include <xpxchaincpp/model/transaction/aggregate_transaction.h>

#include <cassert>

using namespace xpx_chain_sdk;
namespace xpx_chain_sdk { namespace internal {
	
	uint32_t MakeTransactionFullVersion(uint32_t version, NetworkIdentifier networkId)
	{
		return static_cast<uint32_t>(to_underlying_type(networkId) << 24 | version);
	}

	uint32_t ExtractTransactionVersion(uint32_t fullVersion)
	{
		return static_cast<uint32_t>(fullVersion);
	}

	NetworkIdentifier ExtractTransactionNetworkId(uint16_t fullVersion)
	{
		return static_cast<NetworkIdentifier>(fullVersion >> 24);
	}

	RawBuffer GetTransactionSignedData(const Transaction* transaction)
	{
		using namespace xpx_chain_sdk::internal::binary;

		const auto& data = transaction->binary();
		assert(data.size() >= Tx_Signed_Data_Start);

		if (transaction->type() != TransactionType::Aggregate_Complete &&
			transaction->type() != TransactionType::Aggregate_Bonded) {
			return RawBuffer(data.data() + Tx_Signed_Data_Start, data.size() - Tx_Signed_Data_Start);
		} else {
			auto aggregateTx = dynamic_cast<const AggregateTransaction*>(transaction);
			assert(data.size() >= aggregateTx->payloadSize() + Aggregate_Tx_Payload_Start);

			size_t cosignaturesSize = data.size() - (aggregateTx->payloadSize() + Aggregate_Tx_Payload_Start);
			return RawBuffer(data.data() + Tx_Signed_Data_Start, data.size() - Tx_Signed_Data_Start - cosignaturesSize);
		}
	}

	Hash256 CalculateTransactionHash(const Transaction* transaction)
	{
		RawBuffer signatureData = { transaction->signature().data(), transaction->signature().size() / 2 };
		RawBuffer signedData = GetTransactionSignedData(transaction);

		Sha3_256 builder;
		builder.update(signatureData, signedData);
		return builder.finalize();
	}
}}
