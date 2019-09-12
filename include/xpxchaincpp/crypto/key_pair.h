/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/noncopyable.h>
#include <xpxchaincpp/crypto/private_key.h>

namespace xpx_chain_sdk {
	
	/// Pair of private key and associated public key.
	class KeyPair: private MoveOnly {
	public:
		/// Creates a key pair from \a privateKey.
		explicit KeyPair(PrivateKey&& privateKey);
		
		/// Moves key pair from \a rhs.
		KeyPair(KeyPair&& rhs) noexcept;
		
		/// Moves key pair from \a rhs.
		KeyPair& operator=(KeyPair&& rhs) noexcept;
		
		/// Returns a private key of a key pair.
		const PrivateKey& privateKey() const;
		
		/// Returns a public key of a key pair.
		const Key& publicKey() const;
		
	public:
		/// Generates public key from \a privateKey.
		static Key GeneratePublicKey(const PrivateKey& privateKey);
		
	private:
		PrivateKey privateKey_;
		Key publicKey_;
	};
}
