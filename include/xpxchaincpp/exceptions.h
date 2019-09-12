
#pragma once

#include <xpxchaincpp/utils/format.h>

#include <stdexcept>

#define XPX_CHAIN_SDK_DEFINE_EXCEPTION(NAME) \
class NAME: public std::runtime_error { \
	using std::runtime_error::runtime_error; \
}

#define XPX_CHAIN_SDK_THROW(TYPE, MESSAGE) { \
	throw TYPE(MakeString{} << "Exception: " << (MESSAGE)); \
}

#define XPX_CHAIN_SDK_THROW_1(TYPE, MESSAGE, PARAM1) { \
	throw TYPE(MakeString{} << "Exception: " << (MESSAGE) << " (" << (PARAM1) << ")"); \
}

#define XPX_CHAIN_SDK_THROW_2(TYPE, MESSAGE, PARAM1, PARAM2) { \
	throw TYPE(MakeString{} << "Exception: " << (MESSAGE) << " (" << (PARAM1) << ", " << (PARAM2) << ")"); \
}

namespace xpx_chain_sdk {
	
	/// Buffer cast error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(buffer_cast_error);
	
	/// Cryptography error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(crypto_error);
	
	/// Blockchain account error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(account_error);
	
	/// Mosaic error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(mosaic_error);
	
	/// Namespace error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(namespace_error);
	
	/// Transaction error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(transaction_error);
	
	/// Serialization-related error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(serialization_error);

	/// Invalid message error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(message_error);

	/// Invalid format error.
	XPX_CHAIN_SDK_DEFINE_EXCEPTION(format_error);
}
