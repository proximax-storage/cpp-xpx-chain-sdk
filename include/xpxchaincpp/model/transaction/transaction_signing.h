
#pragma once

#include <xpxchaincpp/crypto/key_pair.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/aggregate_transaction.h>


namespace xpx_chain_sdk {
	
	/// Calculates signature for \a transaction using \a keyPair.
	Signature CalculateTransactionSignature(const Transaction* transaction,
	                                        const KeyPair& keyPair);

	/// Calculates aggregate \a transaction cosignature using \a keyPair.
	/// \note Throws \c transaction_error exception if \a transaction is not signed.
	Signature CalculateAggregateTransactionCosignature(const AggregateTransaction* transaction,
	                                                   const KeyPair& keyPair);

	/// Verifies that \a signature is valid \a transaction signature.
	bool VerifyTransactionSignature(const Transaction* transaction,
	                                const Key& signer,
	                                const Signature& signature);

	/// Verifies that \a cosignature is valiad aggregate \a transaction cosignature.
	/// \note Throws \c transaction_error if \a transaction is not signed.
	/// \note Correctly handles situation when \a cosigner corresponds to initial aggregate transaction signer.
	bool VerifyAggregateTransactionCosignature(const AggregateTransaction* transaction,
	                                           const Key& cosigner,
	                                           const Signature& cosignature);

	/// Signs \a transaction and sets its signer and signature parameters.
	void SignTransaction(Transaction* transaction, const KeyPair& keyPair);

	/// Verifies that \a transaction is signed and signature is valid.
	/// \note For aggregate transaction cosignatures are also validated.
	bool VerifyTransaction(const Transaction* transaction);
}
