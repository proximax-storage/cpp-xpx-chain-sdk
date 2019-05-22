
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/buffer.h>
#include <nemcpp/utils/hashable_array.h>

#include <utility>

namespace nem2_sdk {
	
	/// Tag to generate Sha3 hashes.
	struct Sha3Mode {};
	
	/// Tag to generate Keccak hashes.
	struct KeccakMode {};
	
	/// Keccak hash.
	template<typename TMode, size_t HashSize>
	class alignas(32) Keccak {
	public:
		/// Hash size.
		static constexpr size_t Size = HashSize;
		
		/// Hash type.
		using Type = HashableArray<uint8_t, Size>;
		
	public:
		/// Creates Keccak hash builder.
		Keccak();
		
		/// Updates state of hash with all \a args.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it.
		///       Memory occupied by arguments should be available when method \c finalize is called.
		template<typename... TArgs>
		void update(TArgs&&... args)
		{
			(updateState(std::forward<TArgs>(args)), ...);
		}
		
		/// Finalizes computation and returns evaluated hash.
		Type finalize();
		
	public:
		/// Calculates 256-bit SHA3 hash of \a args.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it.
		template<typename... TArgs>
		static Type Hash(const TArgs&... args)
		{
			Keccak keccak;
			keccak.update(args...);
			return keccak.finalize();
		}
		
	private:
		// Updates state of hash (does actual work).
		void updateState(RawBuffer buffer);
		
	private:
		// Raw Keccak_HashInstance data.
		uint8_t hashInstance_[256];
	};
	
	/// Sha3 256-bit hash.
	using Sha3_256 = Keccak<Sha3Mode, Hash256_Size>;
	extern template class Keccak<Sha3Mode, Hash256_Size>;
	
	/// Sha3 512-bit hash.
	using Sha3_512 = Keccak<Sha3Mode, Hash512_Size>;
	extern template class Keccak<Sha3Mode, Hash512_Size>;
	
	/// Keccak 256-bit hash.
	using Keccak_256 = Keccak<KeccakMode, Hash256_Size>;
	extern template class Keccak<KeccakMode, Hash256_Size>;
	
	/// Keccak 512-bit hash.
	using Keccak_512 = Keccak<KeccakMode, Hash512_Size>;
	extern template class Keccak<KeccakMode, Hash512_Size>;
	
	/// 160-bit Ripemd hash.
	class Ripemd_160 {
	public:
		/// Hash size.
		static constexpr size_t Size = Hash160_Size;
		
		/// Hash type.
		using Type = Hash160;
		
	public:
		/// Calculates 160-bit Ripemd hash of \a arg.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it.
		template<typename TArg>
		static Hash160 Hash(const TArg& arg)
		{
			return calculateHash(arg);
		}
	
	private:
		// Calculates buffer's hash (does actual work).
		static Hash160 calculateHash(RawBuffer buffer);
	};
	
	/// 256-bit Sha hash.
	class Sha_256 {
	public:
		/// Hash size.
		static constexpr size_t Size = Hash256_Size;
		
		/// Hash type.
		using Type = Hash256;
		
	public:
		
		/// Calculates 256-bit Sha hash of \a arg.
		/// \note Accepts arguments of type \c RawBuffer or implicitly convertible to it.
		template<typename TArg>
		static Hash256 Hash(const TArg& arg)
		{
			return calculateHash(arg);
		}
	
	private:
		// Calculates buffer's hash (does actual work).
		static Hash256 calculateHash(RawBuffer buffer);
	};
}
