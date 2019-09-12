
#include <xpxchaincpp/utils/format.h>

namespace xpx_chain_sdk {
	
	std::string MakeString::str() const
	{
		return stream_.str();
	}

	MakeString::operator std::string() const
	{
		return str();
	}
}
