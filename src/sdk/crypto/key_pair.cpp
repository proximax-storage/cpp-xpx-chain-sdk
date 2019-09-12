
#include <xpxchaincpp/crypto/key_pair.h>
#include <xpxchaincpp/crypto/hash.h>

extern "C" {
#include "third-party/ref10/ge.h"
}

#include <utility>

namespace xpx_chain_sdk {
	
	KeyPair::KeyPair(PrivateKey&& privateKey):
		privateKey_(std::move(privateKey))
	{
		publicKey_ = GeneratePublicKey(privateKey_);
	}
	
	KeyPair::KeyPair(KeyPair&& rhs) noexcept:
		privateKey_(std::move(rhs.privateKey_)),
		publicKey_(std::move(rhs.publicKey_))
	{ }
	
	KeyPair& KeyPair::operator=(KeyPair&& rhs) noexcept
	{
		privateKey_ = std::move(rhs.privateKey_);
		publicKey_ = std::move(rhs.publicKey_);
		return *this;
	}
	
	const PrivateKey& KeyPair::privateKey() const
	{
		return privateKey_;
	}
	
	const Key& KeyPair::publicKey() const
	{
		return publicKey_;
	}
	
	Key KeyPair::GeneratePublicKey(const PrivateKey& privateKey)
	{
		Key publicKey;
		
		auto hash = Sha3_512::Hash(privateKey);
		hash[0] &= 0xF8;
		hash[31] &= 0x7F;
		hash[31] |= 0x40;
		
		ge_p3 A;
		ge_scalarmult_base(&A, hash.data());
		ge_p3_tobytes(publicKey.data(), &A);
		
		return publicKey;
	}
}
