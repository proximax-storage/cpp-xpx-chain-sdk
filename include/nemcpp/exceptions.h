
#pragma once

#include <nemcpp/utils/format.h>

#include <stdexcept>

#define NEM2_SDK_DEFINE_EXCEPTION(NAME) \
class NAME: public std::runtime_error { \
	using std::runtime_error::runtime_error; \
}

#define NEM2_SDK_THROW(TYPE, MESSAGE) { \
	throw TYPE(MakeString{} << "Exception: " << (MESSAGE)); \
}

#define NEM2_SDK_THROW_1(TYPE, MESSAGE, PARAM1) { \
	throw TYPE(MakeString{} << "Exception: " << (MESSAGE) << " (" << (PARAM1) << ")"); \
}

#define NEM2_SDK_THROW_2(TYPE, MESSAGE, PARAM1, PARAM2) { \
	throw TYPE(MakeString{} << "Exception: " << (MESSAGE) << " (" << (PARAM1) << ", " << (PARAM2) << ")"); \
}

namespace nem2_sdk {
	
	/// Buffer cast error.
	NEM2_SDK_DEFINE_EXCEPTION(buffer_cast_error);
	
	/// Cryptography error.
	NEM2_SDK_DEFINE_EXCEPTION(crypto_error);
	
	/// Blockchain account error.
	NEM2_SDK_DEFINE_EXCEPTION(account_error);
	
	/// Mosaic error.
	NEM2_SDK_DEFINE_EXCEPTION(mosaic_error);
	
	/// Namespace error.
	NEM2_SDK_DEFINE_EXCEPTION(namespace_error);
	
	/// Transaction error.
	NEM2_SDK_DEFINE_EXCEPTION(transaction_error);
	
	/// Serialization-related error.
	NEM2_SDK_DEFINE_EXCEPTION(serialization_error);

	/// Invalid message error.
	NEM2_SDK_DEFINE_EXCEPTION(message_error);
}
