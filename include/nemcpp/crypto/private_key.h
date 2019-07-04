
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/noncopyable.h>

namespace nem2_sdk {
	
	/// Private key.
	/// \note Only a single copy of private key is allowed in memory.
	///       Memory occupied by private key is securely cleared during key pair moving or destruction.
	class PrivateKey: private MoveOnly {
	public:
		/// Creates private key from binary \a data of size \a size.
		/// \note Throws \c crypto_error if \a size is invalid.
		/// \note Memory pointed by \a data is cleared after private key initialization.
		explicit PrivateKey(uint8_t* data, size_t size);
		
		/// Creates private key using \a generator.
		explicit PrivateKey(const supplier<uint8_t>& generator);
		
		/// Moves private key from \a rhs.
		PrivateKey(PrivateKey&& rhs) noexcept;
		
		/// Destroys the private key and securely cleares memory.
		~PrivateKey();
		
		/// Moves private key from \a rhs.
		PrivateKey& operator=(PrivateKey&& rhs) noexcept;
		
		/// Returns private key data.
		const uint8_t* data() const;
		
		/// Returns private key data size.
		size_t size() const;
		
		/// Returns a constant iterator to the beginning of private key data.
		auto begin() const
		{ return key_.begin(); }
		
		/// Returns a constant iterator to the end of private key data.
		auto end() const
		{ return key_.end(); }
		
	private:
		Key key_;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const PrivateKey& lhs, const PrivateKey& rhs);
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const PrivateKey& lhs, const PrivateKey& rhs);
}
