
#include <xpxchaincpp/crypto/hash.h>

extern "C" {
#include "third-party/ripemd160/ripemd160.h"
#include "third-party/sha3/KeccakHash.h"
#include "third-party/sha256/crypto_hash_sha256.h"
}

#include <cstring>

namespace xpx_chain_sdk {
	
	namespace {
		Keccak_HashInstance* CastToKeccakHashInstance(uint8_t* hashInstance)
		{
			return reinterpret_cast<Keccak_HashInstance*>(hashInstance);
		}
		
		void KeccakInitialize(uint8_t* hashInstance, std::integral_constant<size_t, Hash256_Size>)
		{
			Keccak_HashInitialize_SHA3_256(CastToKeccakHashInstance(hashInstance));
		}
		
		void KeccakInitialize(uint8_t* hashInstance, std::integral_constant<size_t, Hash512_Size>)
		{
			Keccak_HashInitialize_SHA3_512(CastToKeccakHashInstance(hashInstance));
		}
		
		void KeccakUpdate(uint8_t* hashInstance, const uint8_t* data, size_t size)
		{
			Keccak_HashUpdate(CastToKeccakHashInstance(hashInstance), data, size * 8);
		}
		
		void KeccakFinalize(uint8_t* hashInstance, uint8_t* hash, uint32_t size, KeccakMode)
		{
			Keccak_HashSqueeze(CastToKeccakHashInstance(hashInstance), hash, size * 8);
		}
		
		void KeccakFinalize(uint8_t* hashInstance, uint8_t* hash, uint32_t /*not used*/, Sha3Mode)
		{
			Keccak_HashFinal(CastToKeccakHashInstance(hashInstance), hash);
		}
	}
	
	template<typename TMode, size_t HashSize>
	Keccak<TMode, HashSize>::Keccak()
	{
		static_assert(sizeof(Keccak_HashInstance) <= sizeof(hashInstance_),
		              "not enough memory for storing Keccak hash instance");
		KeccakInitialize(hashInstance_, std::integral_constant<size_t, HashSize>());
	}
	
	template<typename TMode, size_t HashSize>
	void Keccak<TMode, HashSize>::updateState(RawBuffer buffer)
	{
		KeccakUpdate(hashInstance_, buffer.data(), buffer.size());
	}
	
	template<typename TMode, size_t HashSize>
	typename Keccak<TMode, HashSize>::Type Keccak<TMode, HashSize>::finalize()
	{
		Type hash;
		KeccakFinalize(hashInstance_, hash.data(), static_cast<uint32_t>(hash.size()), TMode{});
		return hash;
	}
	
	template class Keccak<Sha3Mode, Hash256_Size>;
	template class Keccak<Sha3Mode, Hash512_Size>;
	template class Keccak<KeccakMode, Hash256_Size>;
	template class Keccak<KeccakMode, Hash512_Size>;
	
	Hash160 Ripemd_160::calculateHash(RawBuffer buffer)
	{
		Hash160 hash;
		struct ripemd160 context;
		
		ripemd160(&context, buffer.data(), buffer.size());
		std::memcpy(hash.data(), context.u.u8, Hash160_Size);
		return hash;
	}
	
	Hash256 Sha_256::calculateHash(RawBuffer buffer)
	{
		Hash256 hash;
		crypto_hash_sha256_state state;
		
		crypto_hash_sha256_init(&state);
		crypto_hash_sha256_update(&state, buffer.data(), buffer.size());
		crypto_hash_sha256_final(&state, hash.data());
		
		return hash;
	}
}
