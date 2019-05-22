
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/buffer.h>
#include <nemcpp/crypto/key_pair.h>

#include <utility>
#include <vector>

namespace nem2_sdk {
	
	/// Signer/verifier for Curve25519 and 512-bit Sha3/Keccak.
	class Ed25519 {
	public:
		/// Default constructor.
		Ed25519() = default;
		
		/// Adds \a args to a list of signed/verified elements.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it. Memory
		///       occupied by arguments should be available when method \c sign ot \c verify is called.
		template<typename... TArgs>
		void add(TArgs&&... args)
		{
			(addBuffer(std::forward<TArgs>(args)), ...);
		}
		
		/// Removes all elements added for signing/verification.
		void clear();
		
		/// Signs added elements using \a keyPair.
		/// \note The function will throw \c crypto_error if generated S-part of the signature is not
		///       less than the group order.
		Signature sign(const KeyPair& keyPair);
		
		/// Verifies that \a signature of added elements is valid using \a publicKey.
		bool verify(const Key& publicKey, const Signature& signature);
		
	public:
		/// Signs \a buffer using \a keyPair.
		/// \note Throws \c crypto_error if generated S-part of the signature is not less than the group order.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it.
		template<typename... TArgs>
		static Signature Sign(const KeyPair& keyPair, const TArgs&... args)
		{
			Ed25519 builder;
			builder.add(args...);
			return builder.sign(keyPair);
		}
		
		/// Verifies that \a signature of \a TArgs is valid using \a publicKey.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it.
		template<typename... TArgs>
		static bool Verify(const Key& publicKey, const Signature& signature, const TArgs&... args)
		{
			Ed25519 builder;
			builder.add(args...);
			return builder.verify(publicKey, signature);
		}
		
	private:
		// Adds buffer to be signed/verified later.
		void addBuffer(RawBuffer buffer);
		
	private:
		// Contains buffers of all signed/verified elements.
		std::vector<RawBuffer> buffers_;
	};
}
