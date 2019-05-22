
#include <nemcpp/utils/format.h>

namespace nem2_sdk {
	
	MakeString::operator std::string() const
	{
		return stream_.str();
	}
}
