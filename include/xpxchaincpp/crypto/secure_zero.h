/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>

namespace xpx_chain_sdk {
	
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
