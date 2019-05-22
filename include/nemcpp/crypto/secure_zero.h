
#pragma once

#include <nemcpp/utils/buffer.h>

namespace nem2_sdk {
	
	/// Securely cleares \a buffer.
	void SecureZero(MutableRawBuffer buffer);
	
	/// RAII class that securely cleares memory in destructor.
	class SecureZeroGuard {
	public:
		/// Creates guard around \a buffer.
		SecureZeroGuard(MutableRawBuffer buffer);
		
		/// Securely cleares memory.
		~SecureZeroGuard();
		
	private:
		MutableRawBuffer buffer_;
	};
}
