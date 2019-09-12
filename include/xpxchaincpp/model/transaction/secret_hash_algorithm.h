
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/buffer.h>

namespace xpx_chain_sdk {
	
	/// Secret hash algorithm for secret lock/proof transactions.
	enum class SecretHashAlgorithm: uint8_t {
		Op_Sha3_256,
		Op_Keccak_256,
		Op_Bitcoin_160,   // sha256 then ripemd
		Op_Double_Sha_256 // sha256 then sha256
	};
	
	/// Calculates hash for \a secret using \a algorithm.
	/// \note Throws \c crypto_error if \a algorithm is unknown.
	/// \note Hashes with size less then 256 bits are padded with zeros.
	Hash256 CalculateSecretHash(RawBuffer secret, SecretHashAlgorithm algorithm);
}
