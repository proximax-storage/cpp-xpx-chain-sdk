
#include <nemcpp/model/transaction/transaction_signing.h>
#include <nemcpp/crypto/signer.h>

#include "sdk/model/transaction/transaction_utils.h"
#include "infrastructure/binary/offsets.h"
#include "infrastructure/utils/byte_stream.h"

namespace xpx_sdk {
//
//	Signature CalculateTransactionSignature(const Transaction* transaction,
//	                                        const KeyPair& keyPair)
//	{
//		Ed25519 builder;
//		builder.add(internal::GetTransactionSignedData(transaction));
//		return builder.sign(keyPair);
//	}
//
//	Signature CalculateAggregateTransactionCosignature(const AggregateTransaction* transaction,
//	                                                   const KeyPair& keyPair)
//	{
//		Ed25519 builder;
//		builder.add(transaction->hash());
//		return builder.sign(keyPair);
//	}
//
//	bool VerifyTransactionSignature(const Transaction* transaction,
//	                                const Key& signer,
//	                                const Signature& signature)
//	{
//		Ed25519 builder;
//		builder.add(internal::GetTransactionSignedData(transaction));
//		return builder.verify(signer, signature);
//	}
//
//	bool VerifyAggregateTransactionCosignature(const AggregateTransaction* transaction,
//	                                           const Key& cosigner,
//	                                           const Signature& cosignature)
//	{
//		if (cosigner == transaction->signer().publicKey()) {
//			return VerifyTransactionSignature(transaction, cosigner, cosignature);
//		}
//
//		Ed25519 builder;
//		builder.add(transaction->hash());
//		return builder.verify(cosigner, cosignature);
//	}
//
//	void SignTransaction(Transaction* transaction, const KeyPair& keyPair)
//	{
//		transaction->signature_ = CalculateTransactionSignature(transaction, keyPair);
//		transaction->setSigner(keyPair.publicKey());
//
//		// updating binary transaction data
//
//		internal::ByteStream bs(transaction->binary_);
//		bs.seekp(internal::binary::Tx_Signature_Start);
//		bs << *(transaction->signature_) << transaction->signer().publicKey();
//	}
//
//	bool VerifyTransaction(const Transaction* transaction) {
//		if (!transaction->isSigned()) {
//			return false;
//		}
//
//		bool result = true;
//
//		if (transaction->type() != TransactionType::Aggregate_Complete &&
//		    transaction->type() != TransactionType::Aggregate_Bonded) {
//			result = VerifyTransactionSignature(transaction, transaction->signer().publicKey(), transaction->signature());
//		} else {
//			auto aggregateTx = dynamic_cast<const AggregateTransaction*>(transaction);
//
//			for (const auto& [account, signature]: aggregateTx->cosignatures()) {
//				result &= VerifyAggregateTransactionCosignature(aggregateTx, account.publicKey(), signature);
//
//				if (!result) {
//					break;
//				}
//			}
//		}
//
//		return result;
//	}
}
