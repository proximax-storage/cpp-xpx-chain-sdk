/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#define __STDC_WANT_LIB_EXT1__ 1
#include <xpxchaincpp/crypto/secure_zero.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__) || defined(__STDC_LIB_EXT1__)
#include <string.h>
#endif

namespace xpx_chain_sdk {
	
	void SecureZero(MutableRawBuffer buffer)
	{
#ifdef WIN32
		SecureZeroMemory(buffer.data(), buffer.size());
#elif defined(__APPLE__) || defined(__STDC_LIB_EXT1__)
		memset_s(buffer.data(), buffer.size(), 0, buffer.size());
#else
		volatile auto ptr = buffer.data();
		auto size = buffer.size();
		while (size--) *ptr++ = 0;
#endif
	}
	
	SecureZeroGuard::SecureZeroGuard(MutableRawBuffer buffer): buffer_(buffer)
	{ }
	
	SecureZeroGuard::~SecureZeroGuard()
	{
		SecureZero(buffer_);
	}
}
