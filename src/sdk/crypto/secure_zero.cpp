#define __STDC_WANT_LIB_EXT1__ 1
#include <nemcpp/crypto/secure_zero.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__) || defined(__STDC_LIB_EXT1__)
#include <string.h>
#endif

namespace nem2_sdk {
	
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
