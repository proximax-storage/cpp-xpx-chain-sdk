
#include <nemcpp/utils/format.h>

namespace xpx_sdk {
	
	std::string MakeString::str() const
	{
		return stream_.str();
	}

	MakeString::operator std::string() const
	{
		return str();
	}
}
