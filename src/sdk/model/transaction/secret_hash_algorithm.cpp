/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/secret_hash_algorithm.h>
#include <xpxchaincpp/crypto/hash.h>

namespace xpx_chain_sdk {
	
	Hash256 CalculateSecretHash(RawBuffer secret, SecretHashAlgorithm algorithm)
	{
		switch (algorithm) {
		case SecretHashAlgorithm::Op_Sha3_256:
			return Sha3_256::Hash(secret);
		case SecretHashAlgorithm::Op_Keccak_256:
			return Keccak_256::Hash(secret);
		case SecretHashAlgorithm::Op_Bitcoin_160:
			{
				Hash256 hash{};
				std::memcpy(hash.data(), Ripemd_160::Hash(Sha_256::Hash(secret)).data(), Hash160_Size);
				return hash;
			}
		case SecretHashAlgorithm::Op_Double_Sha_256:
			return Sha_256::Hash(Sha_256::Hash(secret));
		default:
			break;
		}
		
		XPX_CHAIN_SDK_THROW_1(crypto_error, "unknown secret hash algorithm", to_underlying_type(algorithm));
	}
}
