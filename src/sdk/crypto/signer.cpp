
#include <xpxchaincpp/crypto/signer.h>
#include <xpxchaincpp/crypto/hash.h>

extern "C" {
#include "third-party/ref10/crypto_verify_32.h"
#include "third-party/ref10/ge.h"
#include "third-party/ref10/sc.h"
}

#include <cstring>

#ifdef _MSC_VER
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

namespace xpx_chain_sdk {
	
	namespace {
		constexpr size_t Encoded_Size = Signature_Size / 2;
		
		// Flags of encoded S-part of signature.
		enum : uint8_t {
			Is_Reduced = 1, // S-part is less than the group order
			Is_Zero = 2     // S-part is zero
		};
		
		uint8_t GetSFlags(const uint8_t* encodedS)
		{
			uint8_t encodedBuf[Signature_Size];
			uint8_t *RESTRICT encodedTempR = encodedBuf;
			uint8_t *RESTRICT encodedZero = encodedBuf + Encoded_Size;
			
			std::memset(encodedZero, 0, Encoded_Size);
			
			if (0 == std::memcmp(encodedS, encodedZero, Encoded_Size)) {
				return Is_Reduced | Is_Zero;
			}
			
			std::memcpy(encodedTempR, encodedS, Encoded_Size);
			sc_reduce(encodedBuf);
			return std::memcmp(encodedTempR, encodedS, Encoded_Size) ? 0 : Is_Reduced;
		}
		
		bool IsCanonicalS(const uint8_t* encodedS) {
			return GetSFlags(encodedS) == Is_Reduced;
		}
		
		void CheckS(const uint8_t* encodedS)
		{
			if (!(GetSFlags(encodedS) & Is_Reduced)) {
				XPX_CHAIN_SDK_THROW(crypto_error, "signature s-part invalid");
			}
		}
	}
	
	void Ed25519::clear()
	{
		buffers_.clear();
	}
	
	Signature Ed25519::sign(const KeyPair& keyPair)
	{
		// Signature is (encodedR, encodedS).
		Signature signature;
		uint8_t* RESTRICT encodedR = signature.data();
		uint8_t* RESTRICT encodedS = signature.data() + Encoded_Size;
		
		// Hash the private key to improve randomness.
		auto privateKeyHash = Sha3_512::Hash(keyPair.privateKey());
		
		// r = H(privateKeyHash[256:512] || data)
		// "EdDSA avoids these issues by generating r = H(h_b, ..., h_2b?1, M), so that
		//  different messages will lead to different, hard-to-predict values of r."
		Sha3_512 rBuilder;
		rBuilder.update(RawBuffer(privateKeyHash.data() + Hash512_Size / 2, Hash512_Size / 2));
		
		for (const auto& buffer: buffers_) {
			rBuilder.update(buffer);
		}
		
		auto rHash = rBuilder.finalize();
		
		// Reduce size of rHash since we are calculating mod group order anyway.
		sc_reduce(rHash.data());
		
		// R = rModQ * base point
		ge_p3 rMulBase;
		ge_scalarmult_base(&rMulBase, rHash.data());
		ge_p3_tobytes(encodedR, &rMulBase);
		
		// h = H(encodedR || publicKey || data)
		Sha3_512 hBuilder;
		hBuilder.update(RawBuffer(encodedR, Encoded_Size), keyPair.publicKey());
		
		for (const auto& buffer: buffers_) {
			hBuilder.update(buffer);
		}
		
		auto hHash = hBuilder.finalize();
		
		// h = h mod group order
		sc_reduce(hHash.data());
		
		// a = field_element(privateKeyHash[0:256])
		privateKeyHash[0] &= 0xF8;
		privateKeyHash[31] &= 0x7F;
		privateKeyHash[31] |= 0x40;
		
		// S = (r + h * a) mod group order
		sc_muladd(encodedS, hHash.data(), privateKeyHash.data(), rHash.data());
		
		// Check result and return.
		CheckS(encodedS);
		return signature;
	}
	
	bool Ed25519::verify(const Key& publicKey, const Signature& signature)
	{
		// Signature is (encodedR, encodedS).
		const uint8_t* RESTRICT encodedR = signature.data();
		const uint8_t* RESTRICT encodedS = signature.data() + Encoded_Size;
		
		if (!IsCanonicalS(encodedS)) {
			return false;
		}
		
		// reject zero public key, which is known weak key
		if (publicKey == Key{}) {
			return false;
		}
		
		// h = H(encodedR || publicKey || data)
		Sha3_512 hBuilder;
		hBuilder.update(RawBuffer(encodedR, Encoded_Size), publicKey);
		
		for (const auto& buffer: buffers_) {
			hBuilder.update(buffer);
		}
		
		auto hHash = hBuilder.finalize();
		
		// h = h mod group order
		sc_reduce(hHash.data());
		
		// A = -pub
		ge_p3 A;
		
		if (ge_frombytes_negate_vartime(&A, publicKey.data()) != 0) {
			return false;
		}
		
		// R = encodedS * B - h * A
		ge_p2 R;
		ge_double_scalarmult_vartime(&R, hHash.data(), &A, encodedS);
		
		// Compare calculated R to given R.
		unsigned char checkR[Encoded_Size];
		ge_tobytes(checkR, &R);
		return crypto_verify_32(checkR, encodedR) == 0;
	}
	
	void Ed25519::addBuffer(RawBuffer buffer)
	{
		buffers_.push_back(buffer);
	}
}
