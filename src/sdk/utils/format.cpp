
#include <nemcpp/utils/format.h>

namespace nem2_sdk {
	
	std::string MakeString::str() const
	{
		return stream_.str();
	}

	MakeString::operator std::string() const
	{
		return str();
	}
}
